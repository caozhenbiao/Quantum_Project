
#ifndef WIN32
#include <unistd.h>
#else
#include <process.h>
#endif 

#include "./src/network.h"
#include "./src/utils.h"
#include "./src/network.h"
#include "./src/region_layer.h"
#include "./src/cost_layer.h"
#include "./src/utils.h"
#include "./src/parser.h"
#include "./src/box.h"
#include "./src/demo.h"
#include "./src/option_list.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "stb_image.h"

network mynet;
char**    mynames;

// compare to sort detection** by bbox.x
int compare_by_lefts_1(const void *a_ptr, const void *b_ptr) {
	const detection_with_class* a = (detection_with_class*)a_ptr;
	const detection_with_class* b = (detection_with_class*)b_ptr;
	const float delta = (a->det.bbox.x - a->det.bbox.w / 2) - (b->det.bbox.x - b->det.bbox.w / 2);
	return delta < 0 ? -1 : delta > 0 ? 1 : 0;
}

// compare to sort detection** by best_class probability
int compare_by_probs_1(const void *a_ptr, const void *b_ptr) {
	const detection_with_class* a = (detection_with_class*)a_ptr;
	const detection_with_class* b = (detection_with_class*)b_ptr;
	float delta = a->det.prob[a->best_class] - b->det.prob[b->best_class];
	return delta < 0 ? -1 : delta > 0 ? 1 : 0;
}

//yolo
static int yolo_setup(const char* nf, const char* cf, const char* wf){
   //const char *  nf = luaL_checkstring(L, 1); //label文件
   //const char *  cf = luaL_checkstring(L, 2); //网络配置文件
   //const char *  wf = luaL_checkstring(L, 3); //权重文件
   printf("darknet setup names:%s   cfg:%s  weight:%s\n", nf, cf, wf);
   int names_size = 0;
   mynames = get_labels_custom((char*)nf, &names_size );
  mynet = parse_network_cfg_custom((char*)cf, 1, 1); // set batch=1
   load_weights(&mynet, (char*)wf);
   //set_batch_network(mynet, 1);
   fuse_conv_batchnorm( mynet );
   calculate_binary_weights( mynet );
   if (mynet.layers[mynet.n - 1].classes != names_size) {
		printf("\n Error: in the file %s number of names %d that isn't equal to classes=%d in the file %s \n",nf, names_size, mynet.layers[mynet.n - 1].classes, cf);
	}
   srand(2222222);
   printf("steup finish!\n");
   return 0;
}

//jpg数据流对象检测
static int framedetect(char*jpgdata, __int64 jpgsize, char** boxlist){
	float thresh = 0.24;
	float hier_thresh = 0.4;
	float nms=.45;
	//JPG数据流转换
	int c = 3;
	int w,h,comp;
	unsigned char * data  = stbi_load_from_memory((unsigned char*)jpgdata,jpgsize,&w,&h,&comp,3);
	if( !data ){
		printf("error image data.\n");
		return 0;
	}
	image im = make_image(w, h, c);
	int ia,ja,k;
	for(k = 0; k < c; ++k){
		for(ja = 0; ja < h; ++ja){
			for(ia = 0; ia < w; ++ia){
				im.data[ia+w*ja+w*h*k] = (float)data[c*ia+c*w*ja]/255.;
			}
		}
	}
	free(data);

	//对象检测
	image sized = letterbox_image(im, mynet.w, mynet.h);
	layer l = mynet.layers[mynet.n - 1];
	float* out = network_predict(mynet, sized.data);
	int nboxes = 0;
	detection *dets = get_network_boxes(&mynet, im.w, im.h, thresh, hier_thresh, 0, 1, &nboxes, 1);
	if (nms) {
		if (l.nms_kind == DEFAULT_NMS) do_nms_sort(dets, nboxes, l.classes, nms);
		else diounms_sort(dets, nboxes, l.classes, nms, l.nms_kind, l.beta_nms);
	}
	//检测输出
	int ndetcnt = 0;
	for (int i = 0; i < nboxes; ++i) {
		for (int j = 0; j < l.classes; ++j) {
			if (dets[i].prob[j] > thresh) {
				box b = dets[i].bbox;
				if (b.x < 0) b.x = 0.0;
				if (b.y < 0) b.y = 0.0;
				char szText[256] = { 0 };
				int n = sprintf(szText, "{\"n\":\"%s\",\"s\":%f ,\"x\":%f,\"y\":%f,\"w\":%f,\"h\":%f}", mynames[j], dets[i].prob[j] * 100, b.x, b.y, b.w, b.h);
				strcat(boxlist, szText);
			}
		}
	}
	//保存检测文件
	if( ndetcnt>0 ){
		free_detections(dets, nboxes);
	}
    free_image(im);
	free_image(sized);
	return 1;
}

int dispath(char* uri, char* data, unsigned data_len, char** data_out) {
	printf("dispath  uri:%s   data:%s len:%d\n", uri, data, data_len);
	*data_out = (char*)malloc(*data_out, 1024);
	memset(*data_out, 0x00, 1024);
	framedetect(data, data_len, data_out);
	return strlen(*data_out);
}

#ifdef _WIN32
char *optarg = NULL;
int getopt(int argc, char **argv, const char *opts) {
	static int sp = 1;
	static int opterr = 1;
	static int optind = 1;
	static int optopt;
	register int c;
	register char *cp;
	if (sp == 1)
		if (optind >= argc || argv[optind][0] != '-' || argv[optind][1] == '\0')
			return(-1);
		else if (strcmp(argv[optind], "--") == 0) {
			optind++;
			return(-1);
		}
	optopt = c = argv[optind][sp];
	if (c == ':' || (cp = (char*)strchr(opts, c)) == 0) {
		printf(": illegal option --%c ", c);
		if (argv[optind][++sp] == '\0') {
			optind++;
			sp = 1;
		}
		return('?');
	}
	if (*++cp == ':') {
		if (argv[optind][sp + 1] != '\0')
			optarg = &argv[optind++][sp + 1];
		else if (++optind >= argc) {
			printf(": option requires an argument --%c ", c);
			sp = 1;
			return('?');
		}
		else
			optarg = argv[optind++];
		sp = 1;
	}
	else {
		if (argv[optind][++sp] == '\0') {
			sp = 1;
			optind++;
		}
		optarg = 0;
	}
	return(c);
}
#endif


#undef main
int main(int argc, char* argv[]) {
	//const char *  nf = luaL_checkstring(L, 1); //label文件
	//const char *  cf = luaL_checkstring(L, 2); //网络配置文件
	//const char *  wf = luaL_checkstring(L, 3); //权重文件
	char nf[256] = { 0 };
	char cf[256] = { 0 };
	char wf[256] = { 0 };
	unsigned short server_port = 9000;
	int c = 0;
	while ((c = getopt(argc, argv, "n:c:w:p")) != -1) {
		switch (c) {
		case 'n': strncpy(nf, optarg, 256); break;
		case 'c': strncpy(cf, optarg, 256); break;
		case 'w': strncpy(wf, optarg, 256); break;
		case 'p': server_port = atoi(optarg); break;
		default:break;
		}
	}
	yolo_setup( nf, cf, wf );
	tcps_start("0.0.0.0", server_port, dispath);
	while (1) {
		static char sz[256] = { 0 };
		if (fgets(sz, 256, stdin) == NULL)
			continue;
		if (strcmp(sz, "exit\n") == 0)
			break;
	}
	return 0;
}

