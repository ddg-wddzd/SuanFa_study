#include<stdio.h>
#include<stdint.h>
#include<string.h>
//1.IP地址结构体(使用位段)
typedef struct{
    unsigned int a : 8;
    unsigned int b : 8;
    unsigned int c : 8;
    unsigned int d : 8;

} IPAddress;
//2.RGB颜色结构体
typedef union{
    struct{
        unsigned int blue : 5;
        unsigned int green : 6;
        unsigned int red : 5;

    } rgb565;
    uint16_t value;
} RGB565;
//3.浮点数分析联合体
typedef union{
    float f;
    struct{
        unsigned int mantissa : 23;
        unsigned int exponent : 8;
        unsigned int sign : 1;
    } parts;
} FloatAnalyzer;
//4.简单的网络数据包头
typedef struct{
    unsigned int version : 4;
    unsigned int ihl : 4;
    unsigned int tos : 8;
    uint16_t total_length;
    uint16_t identification;
    unsigned int flags : 3;
    unsigned int fragment_offset : 13;
    unsigned int ttl : 8;
    unsigned int protocol : 8;
    uint16_t checksum;
    uint32_t src_ip;
    uint32_t dst_ip;
} PacketHeader;

//IP地址转换函数
uint32_t ip_to_int(IPAddress ip){
    return (ip.a << 24) | (ip.b << 16) | (ip.c << 8) | ip.d;
}
IPAddress int_to_ip(uint32_t ip_int ){
    IPAddress ip;
    ip.a = (ip_int >> 24) & 0xff;
    ip.b = (ip_int >> 16) & 0xff;
    ip.c = (ip_int >> 8) & 0xff;
    ip.d = ip_int & 0xff;
    return ip;
}
//RGB888转RGB565
RGB565 rgb888_to_565(uint8_t r,uint8_t g,uint8_t b){
    RGB565 color;
    color.rgb565.red = r >> 3;   // 5位红色
    color.rgb565.green = g >> 2; // 6位绿色
    color.rgb565.blue = b >> 3;  // 5位蓝色
    return color;
}
//RGB565转RGB888