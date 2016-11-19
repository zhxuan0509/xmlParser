#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;

//xml文件基本解析，读取文件把数据放到内存中，并且打印


//判断是否是空格 回车 换行 制表符
bool isspace(char c){

	if(c == ' '|| c == '\t' ||c == '\n'||c == '\r'){
		return true;
	}else{
		return false;
	}
}


struct  Node 
{
	char *name; //记录标签的名字
	char *value; //记录标签的值 
	Node *next;
	Node *prev;
	Node *father;
	Node *sonHead;	
};

class XmlParser
{
private:
	Node *root;
	
	//递归遍历树 输出到输出流（文件、屏幕）
	void traverse(Node *p,ostream &out);
public:
	XmlParser():root(NULL){};
	~XmlParser(){}
	void readFile(const char *filePath);
	void writeFile(const char *filePath){
		ofstream out;
		out.open(filePath);
		traverse(root,out);
		out.close();
	}

	void print(){ 
		traverse(root,cout);
	}
	void insertSon(Node *father,Node *pnew);
};



void XmlParser::traverse(Node *p,ostream &out){
	//静态变量用来记录递归的层次。
	static int deep = 0;
	if(p == NULL){
		return;
	}
	//根据递归来计算对齐空格。
	for(int i = 0;i<deep;i++){
		out<<"  ";
	}

	out<<'<'<<p->name<<'>';
	if(p->value == NULL){
		out<<endl;
	}else{
		out<<p->value;
	}
	Node *tmp = p->sonHead;
	while(tmp != NULL){
		deep++;
		traverse(tmp,out);
		tmp = tmp->next;
	}
	if(p->value == NULL){
		for(int i = 0;i<deep;i++){
			out<<"  ";
		}
	}
	out<<"</"<<p->name<<'>'<<endl;
	//当函数结束的时候递归层次计数--
	deep--;

}





//把p指向的结点，插入到father 指向的结点的下面
void XmlParser::insertSon(Node *father,Node *pnew){
	if(father == NULL){
		root = pnew;
		return;
	}
	
	Node *temp = father->sonHead;
	
	if(temp == NULL){
		father->sonHead = pnew;
		pnew->father = father;
		return;
	}
	//temp->next , temp != NULL
	while(temp->next != NULL){

		temp = temp->next;
	}
	temp->next = pnew;
	pnew->prev = temp;
	pnew->father = father;
}


void XmlParser::readFile(const char *filePath){
	char c = 0;
	char buff[1000];
	int i = 0;
	char flag = 0;
	Node *pcur = NULL;
	//c++中 用ifstream类来读取文件。
	ifstream readFileStream;
	//打开文件，且为只读方式
	readFileStream.open(filePath);

	c = readFileStream.get();
	while(c != EOF ){
	
		if(isspace(c)){
			c = readFileStream.get();
			continue;
		}

		//有标签开始
		if(c == '<'){
	
			c = readFileStream.get();
			if(c == EOF){
				return;
			}

			if(c == '/'){
				//闭合标签
				i = 0;
				//设置buff的数据都为0
				memset(buff,0,sizeof(buff));
				while((c = readFileStream.get() )!= '>'){
					buff[i++] = c;
				}
				if(strcmp(pcur->name,buff) == 0){
					//闭合标签匹配
					pcur = pcur->father;

				}else{
					cout<<"error:闭合标签不匹配 "<<__LINE__<<endl;
				}
				c = readFileStream.get();

			}else{
				//开始标签
				//cout<<"tag start"<<endl;
				i = 0;
				//设置buff的数据都为0
				memset(buff,0,sizeof(buff));

				while(c != '>'){
					buff[i++] = c;
					c = readFileStream.get();
				}

				//得到标签的名字 
				Node *pnew = new Node();
				pnew->name = new char[i+1];
				strcpy(pnew->name,buff);
			

				insertSon(pcur,pnew);
				pcur = pnew;
				c = readFileStream.get();
			}
			
		}else{
				//如果不是标签name,则是标签value 
				i = 0;
				memset(buff,0,sizeof(buff));
				
				while( (c != EOF) && (c != '<')){
					buff[i++] = c;
					c = readFileStream.get();
				}

				pcur->value = new char[i+1];
				strcpy(pcur->value,buff);
				//cout<<pcur->name<<":"<<pcur->value<<endl;
			
		}	
	
	}	


	//关闭文件读取流
	readFileStream.close();
	cout<<"文件读取完毕"<<endl;
}



int main(int argc, char const *argv[])
{
	XmlParser parser;
	parser.readFile("student.xml");
	//把树里的结点输出到屏幕
	parser.print();

	//把树里的结点输出到文件
	parser.writeFile("test.xml");
	
	return 0;
}