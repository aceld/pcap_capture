# pcap_capture

深度报文解析功能，主要解析pcap文件及实时网口抓包及分析工具


# 环境构建依赖

## 1 依赖cmake工具

ubuntu 安装
```bash
    $apt-get install cmake
```

或者其他平台安装cmake即可.


# 2 编译

```bash
cd pcap_capture/

cmake .

make
```


# 3 运行

## 3.1 基本功能测试
```bash
$./pcapc ./pcapfile/eth0.pcap

```
