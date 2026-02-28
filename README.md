基于STM32的嵌入式轻量级日志库，可设置日志等级，显示日志所在文件及行数
文件说明：
	log.h:日志头文件
	log.c:日志源文件
使用时，只需要实现UlogOutputFunc函数，实现串口发送，传入指针即可
使用流程
	设置日志格式:
		Ulog_SetFmt(LOG_FMT_LEVEL_STR  | LOG_FMT_FILE_LINE);
	日志使用
		LOG_TRACE("开始测试日志功能");
   	 	LOG_DEBUG("调试信息 - 变量值: %d", 100);
    		LOG_INFO("系统初始化完成");
		LOG_WARN("内存使用率高达85%%");
	        LOG_ERROR("文件打开失败: %s", "test.log");
	        LOG_FATAL("核心模块初始化失败，系统即将终止");
