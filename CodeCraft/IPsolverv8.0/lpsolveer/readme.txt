���飺D&A	ʱ�䣺2016��4��10��

1��Ŀ¼�ṹ��

��Ŀ¼/
������ code/
��     ������ lib/
��     ��     ������ lib_io.h        
��     ��     ������ lib_record.h     
��     ��     ������ lib_time.h  
��	  ��		������ lp_lib.h        lpsolve������ͷ�ļ� lp_*.h
��     ��     ������ lp_Hash.h     
��     ��     ������ lp_mipbb.h  
��     ��		������ lp_SOS.h        
��     ��     ������ lp_types.h     
��     ��     ������ lp_utils.h
��	  ��		������ lib_matrix.h
��	  ��	    ������ liblpsolve55.a	 lpsolve�����ӿ�
��	  ��		
��	  ������ makefile         		 �����ִ���ļ��� makefile
��     ������ io.cpp
��     ������ future_net.cpp         
��     ������ route.cpp              �Լ�д��Դ�ļ�
��     ������ route.h                �Լ�д��ͷ�ļ�	
��      
������ future_net                   ��ִ���ļ�
������ readme.txt                   ����˵���ļ�readme

2��makefile˵��
	(1)��makefile�����˱������ӱ���������ȫ������
	(2)����code/Ŀ¼��ִ�� make ������� code/ Ŀ¼������ future_net �����ƿ�ִ���ļ�

3����Ȩ˵��
	�����򲿷ֲ�����lpsolve��⣬���� code/lib/lP_*.h �� code/lib/liblpsolve55.a ���Լ���д������lpsolve��������ļ���
	
	���ص�ַ��https://sourceforge.net/projects/lpsolve/