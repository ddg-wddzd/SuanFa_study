a=15
b=4
print(a+b) #加法 输出19
print(a-b) #减法 输出11
print(2.5*4) #乘法 输出10.0(浮点数与整数相乘结果位浮点数)
print(10/2) #除法 输出5.0(除法结果总是浮点数)
print(9/4) #除法 输出2.25
print(10//3) #整数除法 输出3(结果向下取整)
print(-10//3) #整数除法 输出-4(因为 -3.3333333,结果向下取整)
print(10%3) #取余 输出1(10除以3的余数)
print(-10%3) #取余 输出2(因为 -10除以3的商是-4，余数是2)
print(2**3) #幂运算 输出8(2的3次方)
print(9**0.5) #开方 输出3.0(9的平方根)
#   **的优先级高于*和/，所以2**3*4等价于(2**3)*4 输出32
# 优先级从高到低：()  >  **  >  * / // %  >  + - 
# 括号 > 幂 > 正负号 > 乘除取余 > 加减

#示例
result=2+3*4**2 #等价于2+3*(4**2) 输出50
print(result) 
result=(2+3)*4**2 #等价于(2+3)*(4**2) 输出80
print(result)
print(10+20//3*2) #等价于10+(20//3)*2 输出22
# 当优先级相同时，Python 从左到右依次计算（除了幂运算从右到左）
print(10/2*3) #等价于(10/2)*3 输出15.0
print(2**3**2) #等价于2**(3**2) 输出512

#综合示例
#计算圆的面积
radius=5
area=3.14*radius**2 #等价于3.14*(radius**2) 输出78.5
print("面积为：",area)

#判断一个数是否为奇数
num=7
if num%2==1:
    print(f"{num} 是奇数")

#求平均数
scores=[85,92,78,90]
average=sum(scores)/len(scores) #等价于(85+92+78+90)/4 输出86.25
print("平均数为：",average)


name=input("请输入你的名字：") #获取用户输入
print("你好,"+name)
print(f"欢迎,{name}!")

#由于 input() 返回的是字符串，如果我们需要数字（整数、浮点数）来进行数学计算，就必须进行显式类型转换。
#1.转换为整数int()
age_str=input("请输入你的年龄：")
age=int(age_str) #将输入的字符串转换为整数
print(f"明年你就{age+1}岁了")
#2.转换为浮点数float()
height_str=input("请输入你的身高（米）：")
height=float(height_str) #将输入的字符串转换为浮点数
print(f"你的身高是：{height}米")

#3.直接转换的简化写法
age=int(input("请输入年龄："))
height=float(input("请输入身高："))
print(f"年龄：{age},身高：{height}米")

#处理转换错误
#当用户输入的内容无法转换为数字时（例如输入“abc”作为年龄），程序会抛出 ValueError 并终止。为了让程序更健壮，我们可以使用 try...except 捕获异常。
while True:
    try:
        age=int(input("请输入您的年龄："))
        break #如果转换成功，跳出循环
    except ValueError:
        print("输入无效，请输入一个整数。")
print(f"你的年龄是：{age}")

#示例：简单的计算器
try:
    num1=float(input("请输入第一个数字："))
    num2=float(input("请输入第二个数字："))
    result=num1+num2
    print(f"{num1}+{num2}={result}")
except ValueError:
    print("输入无效，请输入数字。")

#如果需要将数字和字符串一起输出，可以使用 str() 转换，或使用 f-string 自动处理。
age=18
print("年龄："+str(age)) #使用 str() 转换整数为字符串
print(f"年龄:{age}") #使用 f-string 自动将 age 转换为字符串并插入输出
#安全性：从用户获取输入时，永远不要假设输入是合法的。始终进行验证和异常处理。


#字符串拼接
#1.使用+运算符
first_name="张"
last_name="三"
full_name=first_name+last_name #字符串连接 输出张三
print(full_name)
#2.使用join()方法
#当需要拼接多个字符串（如列表中的元素）时，join() 更高效、更灵活。
words=["Python","是","一门","语言"]
sentence="".join(words) #使用空字符串作为分隔符连接列表中的字符串 输出Python是一门语言
print(sentence)     #输出：Python 是一门语言
#也可以指定其他分隔符
path="/".join(["home","user","docs"]) #使用斜杠作为分隔符连接列表中的字符串 输出home/user/docs
print(path)       #输出：home/user/docs
#3.使用 f-string 或 format() 进行格式化拼接
name="小明"
age=18
info=f"姓名：{name},年龄：{age}"
print(info) #输出姓名：小明,年龄：18

#字符串切片
#s[start:end:step]
#1.基本切片
s="Hello,Python!"
print(s[0:5]) #输出Hello(索引0到4的字符)
print(s[7:])    #输出Python!(索引7到末尾的字符)
print(s[:5])   #输出Hello(从开头到索引4的字符)
print(s[7:13]) #输出Python(索引7到12的字符)(13是结束索引，不包含在结果中)
#2.负索引
print(s[-7:-1]) #输出Python(从倒数第7个字符到倒数第2个字符)
print(s[-7:])  #输出Python!(从倒数第7个字符到末尾)
#3.步长切片
print(s[::2]) #输出HloPto!(从开头到末尾，每隔一个字符取一个)
print(s[::-1]) #输出!nohtyP,olleH(字符串反转)
#切片不会修改原字符串，而是返回一个新的字符串。原字符串s仍然保持不变。

#字符串替换
#1.使用 replace() 方法
#replace(old,new,count)
text="apple apple apple"
new_text=text.replace("apple","orange")
print(new_text) #输出orange orange orange
#限制替换次数
new_text2=text.replace("apple","orange",2)
print(new_text2) #输出orange orange apple(只替换前两个apple)

#大小写转换
s="Python Programming"
print(s.upper()) #输出PYTHON PROGRAMMING(转换为大写)
print(s.lower()) #输出python programming(转换为小写)
print(s.capitalize()) #输出Python programming(首字母大写)
print(s.title()) #输出Python Programming(每个单词首字母大写)
print(s.swapcase()) #输出pYTHON pROGRAMMING(大小写互换)
#比较字符串时，通常先将两者转为统一大小写，实现不区分大小写的比较：
user_input=input("请输入yes或no：")
if user_input.lower()=="yes":
    print("你选择了是")

text="  Hello,Python!  "
print(text.strip()) #输出Hello,Python!(去除两端的空白)
print(text.split(",")) #输出['  Hello', 'Python!  '](以逗号分割字符串)
print(text.find("Python")) #输出8(查找子字符串的位置，返回索引)

#f-string格式化
name="Alice"
age=18
height=1.75
print(f"姓名:{name},年龄：{age},身高：{height:.2f}米") #输出姓名:Alice,年龄：18,身高：1.75米(身高保留两位小数)
price=5
count=3
print(f"总价：{price*count}元") #输出总价：15元(直接在f-string中进行计算)
print(f"大写：{name.upper()}") #输出大写：ALICE(在f-string中调用方法)
print(f"{10+20}") #输出30(在f-string中直接计算表达式)
