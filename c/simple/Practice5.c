#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_CHARS 256
#define MAX_TREE_NODES 511
#define BUFFER_SIZE 4096

// 霍夫曼树节点
typedef struct HuffmanNode
{
    unsigned char ch;
    int freq;
    struct HuffmanNode *left;
    struct HuffmanNode *right;
} HuffmanNode;

// 优先队列节点
typedef struct
{
    HuffmanNode *node;
} PriorityQueue;

// 霍夫曼编码表
typedef struct
{
    char code[MAX_CHARS];
    int length;
} HuffmanCode;

// 全局变量
int freq[MAX_CHARS] = {0};
HuffmanCode codes[MAX_CHARS];
PriorityQueue pq[MAX_CHARS];
int pq_size = 0;

// 1. 统计字符频率
void count_frequencies(FILE *input)
{
    int ch;
    while ((ch = fgetc(input)) != EOF)
    {
        freq[ch]++;
    }
    rewind(input);
}

// 2. 创建霍夫曼节点
HuffmanNode *create_node(unsigned char ch, int freq)
{
    HuffmanNode *node = (HuffmanNode *)malloc(sizeof(HuffmanNode));
    if (node)
    {
        node->ch = ch;
        node->freq = freq;
        node->left = node->right = NULL;
    }
    return node;
}

// 优先队列操作
void pq_push(HuffmanNode *node)
{
    int i = pq_size;
    pq_size++;

    while (i > 0)
    {
        int parent = (i - 1) / 2;
        if (pq[parent].node->freq <= node->freq)
            break;
        pq[i] = pq[parent];
        i = parent;
    }
    pq[i].node = node;
}

HuffmanNode *pq_pop()
{
    HuffmanNode *result = pq[0].node;
    pq_size--;

    HuffmanNode *last = pq[pq_size].node;
    int i = 0;

    while (i * 2 + 1 < pq_size)
    {
        int child = i * 2 + 1;
        if (child + 1 < pq_size && pq[child + 1].node->freq < pq[child].node->freq)
        {
            child++;
        }
        if (last->freq <= pq[child].node->freq)
            break;
        pq[i] = pq[child];
        i = child;
    }
    pq[i].node = last;

    return result;
}

// 3. 构建霍夫曼树
HuffmanNode *build_huffman_tree()
{
    // 将出现过的字符加入优先队列
    for (int i = 0; i < MAX_CHARS; i++)
    {
        if (freq[i] > 0)
        {
            HuffmanNode *node = create_node(i, freq[i]);
            pq_push(node);
        }
    }

    // 构建霍夫曼树
    while (pq_size > 1)
    {
        HuffmanNode *left = pq_pop();
        HuffmanNode *right = pq_pop();

        HuffmanNode *parent = create_node(0, left->freq + right->freq);
        parent->left = left;
        parent->right = right;

        pq_push(parent);
    }

    return pq_pop();
}

// 4. 生成霍夫曼编码
void generate_codes(HuffmanNode *root, char *code, int depth)
{
    if (root->left == NULL && root->right == NULL)
    {
        code[depth] = '\0';
        strcpy(codes[root->ch].code, code);
        codes[root->ch].length = depth;
        return;
    }

    if (root->left)
    {
        code[depth] = '0';
        generate_codes(root->left, code, depth + 1);
    }

    if (root->right)
    {
        code[depth] = '1';
        generate_codes(root->right, code, depth + 1);
    }
}

// 5. 压缩文件
void compress_file(FILE *input, FILE *output)
{
    // 写入字符频率表
    fwrite(freq, sizeof(int), MAX_CHARS, output);

    // 压缩数据
    unsigned char buffer = 0;
    int bit_count = 0;
    int ch;

    rewind(input);
    while ((ch = fgetc(input)) != EOF)
    {
        char *code = codes[ch].code;
        int len = codes[ch].length;

        for (int i = 0; i < len; i++)
        {
            buffer <<= 1;
            buffer |= (code[i] - '0');
            bit_count++;

            if (bit_count == 8)
            {
                fputc(buffer, output);
                buffer = 0;
                bit_count = 0;
            }
        }
    }

    // 写入最后一个不完整的字节
    if (bit_count > 0)
    {
        buffer <<= (8 - bit_count);
        fputc(buffer, output);
        fputc(bit_count, output); // 写入实际位数
    }
    else
    {
        fputc(0, output); // 写入0表示完整字节
    }
}

// 6. 解压文件
void decompress_file(FILE *input, FILE *output)
{
    // 读取字符频率表
    fread(freq, sizeof(int), MAX_CHARS, input);

    // 重建霍夫曼树
    HuffmanNode *root = build_huffman_tree();

    // 解压数据
    unsigned char buffer;
    int bit_count = 0;
    HuffmanNode *current = root;

    // 读取最后一个字节的位数
    fseek(input, -1, SEEK_END);
    int last_bits = fgetc(input);
    fseek(input, MAX_CHARS * sizeof(int), SEEK_SET);

    long data_end = ftell(input);
    fseek(input, 0, SEEK_END);
    long file_end = ftell(input);
    fseek(input, data_end, SEEK_SET);

    long data_size = file_end - data_end - 1; // 减去最后1字节

    for (long i = 0; i < data_size; i++)
    {
        buffer = fgetc(input);
        int bits = (i == data_size - 1 && last_bits > 0) ? last_bits : 8;

        for (int j = 7; j >= 8 - bits; j--)
        {
            int bit = (buffer >> j) & 1;
            current = bit ? current->right : current->left;

            if (current->left == NULL && current->right == NULL)
            {
                fputc(current->ch, output);
                current = root;
            }
        }
    }

    // 释放树
    // 注意：这里简化了，实际需要递归释放
    free(root);
}

// 释放霍夫曼树
void free_tree(HuffmanNode *root)
{
    if (root)
    {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

int main()
{
    char input_filename[] = "input.txt";
    char compressed_filename[] = "compressed.bin";
    char output_filename[] = "decompressed.txt";

    printf("=== 简单的霍夫曼压缩器 ===\n\n");

    // 1. 打开输入文件
    FILE *input = fopen(input_filename, "r");
    if (!input)
    {
        printf("错误：无法打开输入文件 %s\n", input_filename);
        return 1;
    }

    // 2. 统计字符频率
    count_frequencies(input);
    printf("字符频率统计完成\n");

    // 3. 构建霍夫曼树
    HuffmanNode *root = build_huffman_tree();
    printf("霍夫曼树构建完成\n");

    // 4. 生成编码
    char code[MAX_CHARS];
    generate_codes(root, code, 0);
    printf("霍夫曼编码生成完成\n");

    // 显示部分编码
    printf("\n部分字符编码:\n");
    for (int i = 0; i < 10; i++)
    {
        if (freq[i] > 0)
        {
            printf("字符 %c (ASCII %d): %s\n",
                   i < 32 ? '.' : i, i, codes[i].code);
        }
    }

    // 5. 压缩文件
    FILE *compressed = fopen(compressed_filename, "wb");
    if (!compressed)
    {
        printf("错误：无法创建压缩文件\n");
        fclose(input);
        return 1;
    }

    compress_file(input, compressed);
    fclose(input);
    fclose(compressed);
    printf("文件压缩完成: %s\n", compressed_filename);

    // 6. 解压文件
    FILE *compressed_in = fopen(compressed_filename, "rb");
    if (!compressed_in)
    {
        printf("错误：无法打开压缩文件\n");
        return 1;
    }

    FILE *output = fopen(output_filename, "w");
    if (!output)
    {
        printf("错误：无法创建输出文件\n");
        fclose(compressed_in);
        return 1;
    }

    decompress_file(compressed_in, output);
    fclose(compressed_in);
    fclose(output);
    printf("文件解压完成: %s\n", output_filename);

    // 7. 释放内存
    free_tree(root);

    // 8. 比较文件大小
    FILE *in = fopen(input_filename, "rb");
    FILE *comp = fopen(compressed_filename, "rb");

    if (in && comp)
    {
        fseek(in, 0, SEEK_END);
        fseek(comp, 0, SEEK_END);

        long input_size = ftell(in);
        long compressed_size = ftell(comp);

        float ratio = (1.0 - (float)compressed_size / input_size) * 100;

        printf("\n压缩结果:\n");
        printf("原始文件大小: %ld 字节\n", input_size);
        printf("压缩文件大小: %ld 字节\n", compressed_size);
        printf("压缩率: %.2f%%\n", ratio);

        fclose(in);
        fclose(comp);
    }

    return 0;
}