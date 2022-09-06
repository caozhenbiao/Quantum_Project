
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

#include "../../platform/quark/liblua/src/lua.h"
#include "../../platform/quark/liblua/src/lualib.h"
#include "../../platform/quark/liblua/src/lauxlib.h"

network mynet;
image**  myalphabet = NULL;
char**     mynames;

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
static int setup(lua_State * L){
   const char *  nf = luaL_checkstring(L, 1); //label文件
   const char *  cf = luaL_checkstring(L, 2); //网络配置文件
   const char *  wf = luaL_checkstring(L, 3); //权重文件
   printf("darknet setup names:%s   cfg:%s  weight:%s\n", nf, cf, wf);
   int names_size = 0;
   mynames = get_labels_custom((char*)nf, &names_size );
   myalphabet = load_alphabet();
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
static int framedetect(lua_State * L){
#ifdef WIN32
	 __int64 jpgsize = luaL_checkinteger(L,1);
#else
	int jpgsize = luaL_checkinteger(L, 1);
#endif
	const char* jpgdata = luaL_checklstring(L,2,&jpgsize);
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
	lua_newtable(L);
	//// text output
	//int selected_detections_num;
	//detection_with_class* selected_detections = get_actual_detections(dets, nboxes, thresh, &selected_detections_num, mynames);
	//qsort(selected_detections, selected_detections_num, sizeof(*selected_detections), compare_by_lefts_1);
	//for (int i = 0; i < selected_detections_num; ++i) {
	//	const int best_class = selected_detections[i].best_class;
	//	char szText[256] = { 0 };
	//	sprintf(szText, "{\"n\":\"%s\",\"s\":%f ,\"x\":%f,\"y\":%f,\"w\":%f,\"h\":%f}", mynames[best_class], selected_detections[i].det.prob[best_class] * 100,
	//		selected_detections[i].det.bbox.x,
	//		selected_detections[i].det.bbox.y,
	//		selected_detections[i].det.bbox.w,
	//		selected_detections[i].det.bbox.h);
	//	lua_pushstring(L, szText);
	//	lua_rawseti(L, -2, ndetcnt++);
	//	int j;
	//	for (j = 0; j < l.classes; ++j) {
	//		if (selected_detections[i].det.prob[j] > thresh && j != best_class) {
	//			char szText[256] = { 0 };
	//			sprintf(szText, "{\"n\":\"%s\",\"s\":%f ,\"x\":%f,\"y\":%f,\"w\":%f,\"h\":%f}", mynames[j], selected_detections[i].det.prob[j] * 100,
	//				selected_detections[i].det.bbox.x,
	//				selected_detections[i].det.bbox.y,
	//				selected_detections[i].det.bbox.w,
	//				selected_detections[i].det.bbox.h);
	//			lua_pushstring(L, szText);
	//			lua_rawseti(L, -2, ndetcnt++);
	//		}
	//	}
	//}
	//free(selected_detections);
	for (int i = 0; i < nboxes; ++i) {
		for (int j = 0; j < l.classes; ++j) {
			if (dets[i].prob[j] > thresh) {
				box b = dets[i].bbox;
				if (b.x < 0) b.x = 0.0;
				if (b.y < 0) b.y = 0.0;
				char szText[256] = { 0 };
				sprintf(szText, "{\"n\":\"%s\",\"s\":%f ,\"x\":%f,\"y\":%f,\"w\":%f,\"h\":%f}", mynames[j], dets[i].prob[j] * 100, b.x, b.y, b.w, b.h);
				lua_pushstring(L, szText);
				lua_rawseti(L, -2, ndetcnt++);
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

//filedetect
static int filedetect(lua_State * L ){
    const char* srcfile = luaL_checkstring(L,1);
    const char* dtcfile = luaL_checkstring(L,2);
	float thresh = 0.24;
	float hier_thresh = 0.4;
	float nms=.45;
	image im = load_image((char*)srcfile, 0, 0, mynet.c);
    image sized = letterbox_image(im, mynet.w, mynet.h);
    layer l = mynet.layers[mynet.n-1];
    float* out = network_predict(mynet, sized.data);
	int nboxes = 0;
	printf("start get_network_boxes\n");
	detection *dets = get_network_boxes(&mynet, im.w, im.h, thresh, hier_thresh, 0, 1, &nboxes, 1);
	if (nms) do_nms_sort(dets, nboxes, l.classes, nms);
	printf("filedetect finish boxs:%d class:%d\n", nboxes, l.classes);
	//检测输出
	int ndetcnt = 0;
	lua_newtable(L);
	for (int i = 0; i < nboxes; ++i) {
		for (int j = 0; j < l.classes; ++j) {
			if (dets[i].prob[j] > thresh) {
				box b = dets[i].bbox;
				if (b.x < 0) b.x = 0.0;
				if (b.y < 0) b.y = 0.0;
				char szText[256] = { 0 };
				sprintf(szText, "{\"n\":\"%s\",\"s\":%f ,\"x\":%f,\"y\":%f,\"w\":%f,\"h\":%f}", mynames[j], dets[i].prob[j] * 100, b.x, b.y, b.w, b.h);
				lua_pushstring(L, szText);
				lua_rawseti(L, -2, ndetcnt++);
			}
		}
	}
   if( ndetcnt>0 ){
		draw_detections_v3(im, dets, nboxes, thresh, mynames, myalphabet, l.classes, 1);
        free_detections(dets, nboxes);
		save_image(im, dtcfile);
    }
    free_image(im);
    free_image(sized);
    return 1; 
}

//train
static int train(lua_State * L ){
	const char*  datacfg  = luaL_checkstring(L,1);   //数据配置文件，包括几个文件信息
	const char*  cfgfile  = luaL_checkstring(L,2);   //网络配置文件
 	const char*  weightfile  = luaL_checkstring(L,3);//权重文件

    list *options = read_data_cfg((char*)datacfg);
    char *train_images = option_find_str(options, (char*)"train", (char*)"data/train.txt");
    char *backup_directory = option_find_str(options, (char*)"backup", (char*)"/backup/");

	srand(time(0));
	char *base = basecfg(cfgfile);
	printf("%s\n", base);
	float avg_loss = -1;
	network net = parse_network_cfg(cfgfile);
	if (weightfile) {
		load_weights(&net, weightfile);
	}
	printf("Learning Rate: %g, Momentum: %g, Decay: %g\n", net.learning_rate, net.momentum, net.decay);
	int imgs = net.batch*net.subdivisions;
	int i = *net.seen / imgs;
	data train, buffer;
	layer l = net.layers[net.n - 1];
	int side = l.side;
	int classes = l.classes;
	float jitter = l.jitter;

	list *plist = get_paths(train_images);
	char **paths = (char **)list_to_array(plist);

	load_args args = { 0 };
	args.w = net.w;
	args.h = net.h;
	args.paths = paths;
	args.n = imgs;
	args.m = plist->size;
	args.classes = classes;
	args.jitter = jitter;
	args.num_boxes = side;
	args.d = &buffer;
	args.type = REGION_DATA;

	args.angle = net.angle;
	args.exposure = net.exposure;
	args.saturation = net.saturation;
	args.hue = net.hue;

	pthread_t load_thread = load_data_in_thread(args);
	clock_t time;
	//while(i*imgs < N*120){
	while (get_current_batch(net) < net.max_batches) {
		i += 1;
		time = clock();
		pthread_join(load_thread, 0);
		train = buffer;
		load_thread = load_data_in_thread(args);
		printf("Loaded: %lf seconds\n", sec(clock() - time));

		time = clock();
		float loss = train_network(net, train);
		if (avg_loss < 0) avg_loss = loss;
		avg_loss = avg_loss * .9 + loss * .1;
		printf("%d: %f, %f avg, %f rate, %lf seconds, %d images\n", i, loss, avg_loss, get_current_rate(net), sec(clock() - time), i*imgs);

		if (i % 1000 == 0 || (i < 1000 && i % 100 == 0)) {
			char buff[256];
			sprintf(buff, "%s/%s_%d.weights", backup_directory, base, i);
			save_weights(net, buff);
		}
		free_data(train);
	}
	char buff[256];
	sprintf(buff, "%s/%s_final.weights", backup_directory, base);
	save_weights(net, buff);
	return 0;
}

//yolo对像销毁
static int destory(lua_State * L ){
	printf( "destory" );
	return 0;
}

static const struct luaL_Reg myLib[] = {
	{"setup", setup},
	{"framedetect", framedetect},
	{"filedetect", filedetect},
	{"train", train},
	{"destory", destory},
	{NULL, NULL},
};

//luaopen_darknet
#ifdef WIN32
	__declspec(dllexport) int luaopen_darknet(lua_State *L) {
#else
	int luaopen_darknet(lua_State *L) {
#endif
	printf("luaopen_darknet\n");
	lua_newtable(L);
	const luaL_Reg * la = &myLib[0];
	for(;la->name!=NULL; la++){
		lua_pushcclosure(L,la->func,0);
		lua_setfield(L,-2,la->name);
	}
	lua_pop(L,0);
	return 1;
}

/*
#undef main
int main(int argc, char* argv[]) {

printf("start main.......\n");
return 0;
}
*/
