print("Hello,World!")  #输出字符串
print(1223)  #输出整数
print(3.14) #输出浮点数
print(True)  #输出布尔值
print("年龄",18,"岁") #输出多个值，逗号分隔，默认空格分隔
print("苹果","香蕉","橘子",sep=",") #输出多个值，逗号分隔，sep参数指定分隔符
print("第一行",end="")
print("仍在第一行") #输出不换行，end参数指定结束符
name="张三"#字符串
age=18#整数
height=1.75#浮点数
is_student=True#布尔值
x,y,z=1,2,3#多变量赋值
a=b=c=10#都赋值位10
data=100#整数类型
data="文本"#现在变为字符串
a=42
b=0b101010 #二进制表示42
c=0o52 #八进制表示42
d=0x2A #十六进制表示42
print(a,b,c,d) #输出十进制、二进制、八进制、十六进制的值
pi=3.14159 #浮点数
distance=1.5e8 #科学计数法表示1.5乘以10的8次方
print(0.1+0.2) #浮点数运算可能存在精度问题 输出0.30000000000000004
s1="Hello"
s2="World"
s3="""多行
字符串""" #三引号表示多行字符串
print(s1[0]) #输出字符串的第一个字符H
print(s1[2:5]) #输出字符串的第2到第4个字符llo
print(s1+s2) #字符串连接 输出HelloWorld
flag=True
if flag:
    print("条件成立")
print(True+5) #布尔值True在数值运算中等于1 输出6
#隐式转换
x=10 #整数
y=3.14 #浮点数
z=x+y #整数和浮点数相加，整数被隐式转换为浮点数 输出13.14
print(type(z)) #输出z的类型 <class 'float'>
#显式转换(强制转换)
#int() 将值转换为整数
print(int(3.14)) #输出3
print(int("42")) #输出42
print(int(True)) #输出1
print(int(False)) #输出0
# int("3.14") #会报错，无法将字符串直接转换为整数
#float() 将值转换为浮点数
print(float(10)) #输出10.0
print(float("3.14")) #输出3.14
print(float("5")) #输出5.0
print(float(True)) #输出1.0
#str() 将值转换为字符串
print(str(123)) #输出'123'
print(str(3.14)) #输出'3.14'
print(str(True)) #输出'True'
#bool() 将值转换为布尔值
print(bool(0)) #输出False
print(bool(1)) #输出True
print(bool("Python")) #输出True 非空字符串为True
print(bool([])) #输出False 空列表为False
#浮点数转整数时，直接截断小数部分（不是四舍五入），如果需要四舍五入，可使用round()函数
#安全转换
num_str=input("请输入一个数字：")
try:
    num=float(num_str) #尝试将输入转换为浮点数
    print("转化成功",num)
except ValueError:
    print("输入不是有效的数字")
