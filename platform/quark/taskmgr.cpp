#include "taskmgr.h"

scoped_ptr<base::Thread> g_thread[taskmgr::ID_COUNT];

void taskmgr::start() {
	base::Thread::Options options;
	options.message_loop_type = base::MessageLoop::TYPE_IO;
	for (int i = 0; i < ID_COUNT; i++) {
		g_thread[i].reset(new base::Thread("Thread_"+std::to_string(i)));
		g_thread[i]->StartWithOptions(options);
	}
}

void taskmgr::stop() {
	for (int i = 0; i < ID_COUNT;i++) {
		g_thread[i]->Stop();
		g_thread[i].reset();
	}
}

void taskmgr::run(bool console) {
	while ( console ) {
		printf("cmd>");
		static char sz[256] = { 0 };
		if (fgets(sz, 256, stdin) == NULL)
			continue;
		if (strcmp(sz, "exit\n") == 0)
			return;
	}
	base::MessageLoop main_loop;
	base::RunLoop run_loop;
	run_loop.Run();
}

bool taskmgr::posttask(ID identifier, const base::Closure& task) {
	assert(identifier < ID_COUNT);
	return g_thread[identifier]->message_loop_proxy()->PostTask(
		FROM_HERE,
		task);
}

bool taskmgr::postdelayedtask(ID identifier, const base::Closure& task, base::TimeDelta delay) {
	assert(identifier < ID_COUNT);
	return g_thread[identifier]->message_loop_proxy()->PostDelayedTask(
		FROM_HERE,
		task,
		delay);
}

bool taskmgr::posttaskreplay(ID identifier, const base::Closure& task, const base::Closure& reply) {
	assert(identifier < ID_COUNT);
	return g_thread[identifier]->message_loop_proxy()->PostTaskAndReply(
		FROM_HERE,
		task,
		reply);
}
