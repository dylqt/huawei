队伍：D&A	时间：2016年4月10日

1、目录结构：

根目录/
├── code/
│     ├── lib/
│     │     │── lib_io.h        
│     │     │── lib_record.h     
│     │     │── lib_time.h  
│	  │		│── lp_lib.h        lpsolve的依赖头文件 lp_*.h
│     │     │── lp_Hash.h     
│     │     │── lp_mipbb.h  
│     │		│── lp_SOS.h        
│     │     │── lp_types.h     
│     │     │── lp_utils.h
│	  │		│── lib_matrix.h
│	  │	    │── liblpsolve55.a	 lpsolve的连接库
│	  │		
│	  ├── makefile         		 编译可执行文件的 makefile
│     ├── io.cpp
│     ├── future_net.cpp         
│     ├── route.cpp              自己写的源文件
│     └── route.h                自己写的头文件	
│      
├── future_net                   可执行文件
└── readme.txt                   程序说明文件readme

2、makefile说明
	(1)、makefile包含了编译连接本程序代码的全部操作
	(2)、在code/目录下执行 make 命令，会在 code/ 目录下生成 future_net 二进制可执行文件

3、版权说明
	本程序部分采用了lpsolve求解，其中 code/lib/lP_*.h 和 code/lib/liblpsolve55.a 非自己编写，属于lpsolve库的依赖文件。
	
	下载地址：https://sourceforge.net/projects/lpsolve/