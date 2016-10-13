操作系统：　centos 7
数据库：　mysql
编译命令: gcc $(mysql_config --cflags) socket_server.c -o conn $(mysql_config --libs);
