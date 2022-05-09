
#ifndef WIN32
#include <unistd.h>
#else
#include <process.h>
#endif 

#include "./src/network.h"
#include "./src/utils.h"
#include "../../platform/quark/liblua/src/lua.h"
#include "../../platform/quark/liblua/src/lualib.h"
#include "../../platform/quark/liblua/src/lauxlib.h"


network* mynet = NULL;
image**  myalphabet = NULL;
char**   mynames;

//yolo
static int setup(lua_State * L){
   printf("yolo setup thread start\n");
   const char *  nf = luaL_checkstring(L, 1); //label�ļ�
   const char *  cf = luaL_checkstring(L, 2); //���������ļ�
   const char *  wf = luaL_checkstring(L, 3); //Ȩ���ļ�
   mynames = get_labels((char*)nf);
   myalphabet = load_alphabet();
   mynet = parse_network_cfg((char*)cf);
   load_weights(mynet, (char*)wf);
   set_batch_network(mynet, 1);
   srand(2222222);
   printf("steup finish!");
   return 0;
}

//jpg������������
static int framedetect(lua_State * L){
	int jpgsize = luaL_checkinteger(L,1);
	const char* jpgdata = luaL_checklstring(L,2,&jpgsize);
	float thresh = 0.24;
	float hier_thresh = 0.5;
	float nms=.45;

	//JPG������ת��
	int c = 3;
	int w,h,comp;
	//unsigned char * data  = stbi_load_from_memory((unsigned char*)jpgdata,jpgsize,&w,&h,&comp,3);					  
	unsigned char * data  = stream_toimage_data((unsigned char*)jpgdata,jpgsize,&w,&h);
	if( !data ){
		printf("error image data.\n");
		return 0;
	}
	image im = make_image(w, h, c);
	int i,j,k;
	for(k = 0; k < c; ++k){
		for(j = 0; j < h; ++j){
			for(i = 0; i < w; ++i){
				im.data[i+w*j+w*h*k] = (float)data[c*i+c*w*j]/255.;
			}
		}
	}
	free(data);
 
	//������
    image sized = letterbox_image(im, mynet->w, mynet->h);
    layer l = mynet->layers[mynet->n-1];
    network_predict(*mynet, sized.data);
	int nboxes = 0;
	detection *dets = NULL; //= get_network_boxes(mynet, im.w, im.h, thresh, hier_thresh, 0, 1, &nboxes);
	if (nms) do_nms_sort(dets, nboxes, l.classes, nms);

	printf("get_network_boxes finish boxs:%d class:%d\n", nboxes, l.classes);

	//������
	int ndetcnt = 0;
	lua_newtable(L);
	int ib,jb;
	for( ib = 0; ib < nboxes; ++ib){
        for( jb = 0; jb < l.classes; ++jb){
            if (dets[ib].prob[jb] > thresh){
				box b = dets[ib].bbox;
				int left  = (b.x-b.w/2.)*im.w;
				int right = (b.x+b.w/2.)*im.w;
				int top   = (b.y-b.h/2.)*im.h;
				int bot   = (b.y+b.h/2.)*im.h;
				if(left < 0) left = 0;
				if(right > im.w-1) right = im.w-1;
				if(top < 0) top = 0;
				if(bot > im.h-1) bot = im.h-1;
				char szText[512]={0};
				sprintf(szText,"name:%s simal:%f left:%d right:%d top:%d bottom:%d",mynames[jb], dets[ib].prob[jb]*100, left,right,top,bot);
				lua_pushstring(L,szText);
				lua_rawseti(L,-2,ndetcnt++);
				printf("%s\n",szText);
            }
        }
    }

	//�������ļ�
	if( ndetcnt>0 ){
	//	draw_detections(im, dets, nboxes, thresh, mynames, myalphabet, l.classes);
		free_detections(dets, nboxes);
		static int ii = 0;
		char path[256];
		sprintf(path, "./%d", ii++ );
		save_image(im, path);
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
    image im = load_image_color((char*)srcfile,0,0);
    image sized = letterbox_image(im, mynet->w, mynet->h);
    layer l = mynet->layers[mynet->n-1];
    float* out = network_predict(*mynet, sized.data);
	int nboxes = 0;
	printf("start get_network_boxes\n");
	detection *dets = NULL; // = get_network_boxes(mynet, im.w, im.h, thresh, hier_thresh, 0, 1, &nboxes);

 
	printf("start do_nms_sort\n");

	if (nms) do_nms_sort(dets, nboxes, l.classes, nms);
	printf("filedetect finish boxs:%d class:%d\n", nboxes, l.classes);
	//������
	int ndetcnt = 0;
	lua_newtable(L);
	int i, j;
	for(i = 0; i < nboxes; ++i){
        for(j = 0; j < l.classes; ++j){
            if (dets[i].prob[j] > thresh){
				box b = dets[i].bbox;
				int left  = (b.x-b.w/2.)*im.w;
				int right = (b.x+b.w/2.)*im.w;
				int top   = (b.y-b.h/2.)*im.h;
				int bot   = (b.y+b.h/2.)*im.h;
				if(left < 0) left = 0;
				if(right > im.w-1) right = im.w-1;
				if(top < 0) top = 0;
				if(bot > im.h-1) bot = im.h-1;
				char szText[256]={0};
				sprintf(szText,"name:%s simal:%f left:%d right:%d top:%d bottom:%d",mynames[j], dets[i].prob[j]*100, left,right,top,bot);
				lua_pushstring(L,szText);
				lua_rawseti(L,-2,ndetcnt++);
				printf("%s\n",szText);
				ndetcnt++;
            }
        }
    }
   if( ndetcnt>0 ){
	   // draw_detections(im, dets, nboxes, thresh, mynames, myalphabet, l.classes);


	//	void draw_detections(image im, int num, float thresh, box *boxes, float **probs, char **names, image **alphabet, int classes)


        free_detections(dets, nboxes);
		save_image(im, dtcfile);
    }
    free_image(im);
    free_image(sized);
    return 1; 
}

//train
static int train(lua_State * L ){
	const char*  datacfg  = luaL_checkstring(L,1);   //���������ļ������������ļ���Ϣ
	const char*  cfgfile  = luaL_checkstring(L,2);   //���������ļ�
 	const char*  weightfile  = luaL_checkstring(L,3);//Ȩ���ļ�
	int ngpus = 0;
	int clear = 0;

    list *options = read_data_cfg((char*)datacfg);
    char *train_images = option_find_str(options, (char*)"train", (char*)"data/train.list");
    char *backup_directory = option_find_str(options, (char*)"backup", (char*)"/backup/");

    srand(time(0));
    char *base = basecfg((char*)cfgfile);
    printf("%s\n", base);
    float avg_loss = -1;
    network **nets = calloc(ngpus, sizeof(network));

    srand(time(0));
    int seed = rand();
    int i;
    for(i = 0; i < ngpus; ++i){
        srand(seed);
#ifdef GPU
       // cuda_set_device(gpus[i]);
#endif
        nets[i] = load_network((char*)cfgfile, (char*)weightfile, clear);
        nets[i]->learning_rate *= ngpus;
    }
    srand(time(0));
    network *net = nets[0];

    int imgs = net->batch * net->subdivisions * ngpus;
    printf("Learning Rate: %g, Momentum: %g, Decay: %g\n", net->learning_rate, net->momentum, net->decay);
    data train, buffer;

    layer l = net->layers[net->n - 1];

    int classes = l.classes;
    float jitter = l.jitter;

    list *plist = get_paths(train_images);
    //int N = plist->size;
    char **paths = (char **)list_to_array(plist);

    load_args args = get_base_args(net);
    args.coords = l.coords;
    args.paths = paths;
    args.n = imgs;
    args.m = plist->size;
    args.classes = classes;
    args.jitter = jitter;
    args.num_boxes = l.max_boxes;
    args.d = &buffer;
    args.type = DETECTION_DATA;
    //args.type = INSTANCE_DATA;
    args.threads = 64;

    pthread_t load_thread = load_data(args);
    double time;
    int count = 0;
    //while(i*imgs < N*120){
    while(get_current_batch(*net) < net->max_batches){
        if(l.random && count++%10 == 0){
            printf("Resizing\n");
            int dim = (rand() % 10 + 10) * 32;
            if (get_current_batch(*net)+200 > net->max_batches) dim = 608;
            //int dim = (rand() % 4 + 16) * 32;
            printf("%d\n", dim);
            args.w = dim;
            args.h = dim;

            pthread_join(load_thread, 0);
            train = buffer;
            free_data(train);
            load_thread = load_data(args);

            #pragma omp parallel for
            for(i = 0; i < ngpus; ++i){
                resize_network(nets[i], dim, dim);
            }
            net = nets[0];
        }
        time=what_time_is_it_now();
        pthread_join(load_thread, 0);
        train = buffer;
        load_thread = load_data(args);
        printf("Loaded: %lf seconds\n", what_time_is_it_now()-time);

        time=what_time_is_it_now();
        float loss = 0;
#ifdef GPU
        if(ngpus == 1){
            loss = train_network(net, train);
        } else {
            loss = train_networks(nets, ngpus, train, 4);
        }
#else
        loss = train_network(*net, train);
#endif
        if (avg_loss < 0) avg_loss = loss;
        avg_loss = avg_loss*.9 + loss*.1;

        i = get_current_batch(*net);
        printf("%ld: %f, %f avg, %f rate, %lf seconds, %d images\n", get_current_batch(*net), loss, avg_loss, get_current_rate(*net), what_time_is_it_now()-time, i*imgs);
        if(i%100==0){
#ifdef GPU
            if(ngpus != 1) sync_nets(nets, ngpus, 0);
#endif
            char buff[256];
            sprintf(buff, "%s/%s.backup", backup_directory, base);
            save_weights(net, buff);
        }
        if(i%10000==0 || (i < 1000 && i%100 == 0)){
#ifdef GPU
            if(ngpus != 1) sync_nets(nets, ngpus, 0);
#endif
            char buff[256];
            sprintf(buff, "%s/%s_%d.weights", backup_directory, base, i);
            save_weights(net, buff);
        }
        free_data(train);
    }
#ifdef GPU
    if(ngpus != 1) sync_nets(nets, ngpus, 0);
#endif
    char buff[256];
    sprintf(buff, "%s/%s_final.weights", backup_directory, base);
    save_weights(net, buff);
	return 0;
}

//yolo��������
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

//open

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
