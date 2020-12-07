# codesim 171240524 张灵毓

## 编译说明

所需要的依赖库有`clang-7(7.0.1)`,`libclang-7-dev(7.0.1)`, 这两个库于2018年发布，一般在linux系统下可以直接通过apt安装。

```shell
sudo apt install clang-7
sudo apt install libclang-7-dev
```

可以在`/etc/apt/sources.list`文件中添加下面一行，然后update后去下载上面的依赖库

```shell
deb http://ftp.cn.debian.org/debian buster main 
```

安装完依赖库后可直接在codesim-Moss目录下make即可完成编译。
