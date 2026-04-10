#include <stdio.h>
#include <stdint.h>
#include <string.h>

// 1. IP地址结构体（使用位段）
typedef struct
{
    unsigned int a : 8;
    unsigned int b : 8;
    unsigned int c : 8;
    unsigned int d : 8;
} IPAddress;

// 2. RGB颜色结构体
typedef union
{
    struct
    {
        unsigned int blue : 5;  // RGB565: 蓝色5位
        unsigned int green : 6; // 绿色6位
        unsigned int red : 5;   // 红色5位
    } rgb565;
    uint16_t value; // 整个16位值
} RGB565;

// 3. 浮点数分析联合体
typedef union
{
    float f;
    struct
    {
        unsigned int mantissa : 23; // 尾数
        unsigned int exponent : 8;  // 指数
        unsigned int sign : 1;      // 符号位
    } parts;
} FloatAnalyzer;

// 4. 简单的网络数据包头
typedef struct
{
    unsigned int version : 4;          // 版本号
    unsigned int ihl : 4;              // 头部长度
    unsigned int tos : 8;              // 服务类型
    uint16_t total_length;             // 总长度
    uint16_t identification;           // 标识
    unsigned int flags : 3;            // 标志
    unsigned int fragment_offset : 13; // 分片偏移
    unsigned int ttl : 8;              // 生存时间
    unsigned int protocol : 8;         // 协议
    uint16_t checksum;                 // 校验和
    uint32_t src_ip;                   // 源IP
    uint32_t dst_ip;                   // 目的IP
} PacketHeader;

// IP地址转换函数
uint32_t ip_to_int(IPAddress ip)
{
    return (ip.a << 24) | (ip.b << 16) | (ip.c << 8) | ip.d;
}

IPAddress int_to_ip(uint32_t ip_int)
{
    IPAddress ip;
    ip.a = (ip_int >> 24) & 0xFF;
    ip.b = (ip_int >> 16) & 0xFF;
    ip.c = (ip_int >> 8) & 0xFF;
    ip.d = ip_int & 0xFF;
    return ip;
}

// RGB888转RGB565
RGB565 rgb888_to_565(uint8_t r, uint8_t g, uint8_t b)
{
    RGB565 color;
    color.rgb565.red = r >> 3;   // 5位红色
    color.rgb565.green = g >> 2; // 6位绿色
    color.rgb565.blue = b >> 3;  // 5位蓝色
    return color;
}

// RGB565转RGB888
void rgb565_to_888(RGB565 color, uint8_t *r, uint8_t *g, uint8_t *b)
{
    *r = (color.rgb565.red << 3) | (color.rgb565.red >> 2);
    *g = (color.rgb565.green << 2) | (color.rgb565.green >> 4);
    *b = (color.rgb565.blue << 3) | (color.rgb565.blue >> 2);
}

// 分析浮点数
void analyze_float(float num)
{
    FloatAnalyzer fa;
    fa.f = num;

    printf("浮点数: %f\n", fa.f);
    printf("十六进制: 0x%08X\n", *(uint32_t *)&fa.f);
    printf("符号位: %d (%s)\n",
           fa.parts.sign,
           fa.parts.sign ? "负数" : "正数");
    printf("指数: %d (实际: %d)\n",
           fa.parts.exponent,
           fa.parts.exponent - 127);
    printf("尾数: 0x%06X\n", fa.parts.mantissa);

    // 计算实际值
    int sign = fa.parts.sign ? -1 : 1;
    int exponent = fa.parts.exponent - 127;
    float mantissa = 1.0 + (fa.parts.mantissa / (float)(1 << 23));
    float value = sign * mantissa * (1 << exponent);
    printf("计算值: %f\n\n", value);
}

int main()
{
    printf("=== 位段和联合体应用示例 ===\n\n");

    // 1. IP地址转换
    printf("1. IP地址转换:\n");
    IPAddress ip = {192, 168, 1, 100};
    printf("IP地址: %u.%u.%u.%u\n", ip.a, ip.b, ip.c, ip.d);

    uint32_t ip_int = ip_to_int(ip);
    printf("转换为整数: 0x%08X (%u)\n", ip_int, ip_int);

    IPAddress ip2 = int_to_ip(ip_int);
    printf("转换回IP: %u.%u.%u.%u\n\n", ip2.a, ip2.b, ip2.c, ip2.d);

    // 2. RGB颜色转换
    printf("2. RGB颜色转换:\n");
    uint8_t r = 255, g = 128, b = 64;
    printf("RGB888: (%d, %d, %d)\n", r, g, b);

    RGB565 color = rgb888_to_565(r, g, b);
    printf("RGB565: 0x%04X\n", color.value);

    uint8_t r2, g2, b2;
    rgb565_to_888(color, &r2, &g2, &b2);
    printf("转回RGB888: (%d, %d, %d)\n\n", r2, g2, b2);

    // 3. 浮点数分析
    printf("3. 浮点数分析:\n");
    analyze_float(3.14f);
    analyze_float(-2.5f);
    analyze_float(0.0f);

    // 4. 网络数据包解析
    printf("4. 模拟网络数据包解析:\n");

    // 创建一个示例数据包
    uint8_t packet[20] = {
        0x45, 0x00, 0x00, 0x28, // 版本/头部长度/服务类型/总长度
        0x12, 0x34, 0x40, 0x00, // 标识/标志/分片偏移
        0x40, 0x06, 0x00, 0x00, // TTL/协议/校验和
        0xC0, 0xA8, 0x01, 0x01, // 源IP: 192.168.1.1
        0xC0, 0xA8, 0x01, 0x64  // 目的IP: 192.168.1.100
    };

    PacketHeader *header = (PacketHeader *)packet;

    printf("版本: %d\n", header->version);
    printf("头部长度: %d 字节\n", header->ihl * 4);
    printf("总长度: %d 字节\n", header->total_length);
    printf("协议: %d\n", header->protocol);

    // 提取源IP和目的IP
    IPAddress src_ip = int_to_ip(header->src_ip);
    IPAddress dst_ip = int_to_ip(header->dst_ip);
    printf("源IP: %u.%u.%u.%u\n",
           src_ip.a, src_ip.b, src_ip.c, src_ip.d);
    printf("目的IP: %u.%u.%u.%u\n",
           dst_ip.a, dst_ip.b, dst_ip.c, dst_ip.d);

    return 0;
}