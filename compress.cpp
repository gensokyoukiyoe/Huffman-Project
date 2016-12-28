#include<iostream>
#include<malloc.h>
#include<string.h>
#include<algorithm>
#include<map>
#include<vector>
#define ll long long

using namespace std;

typedef struct {
    int  sign;
	int weight;
	int parent;
	int lchild;
	int rchild;
}HTNode;
typedef HTNode *HuffmanTree;

typedef struct{
	int name;
	int cnt;
}Code;


void Select(HuffmanTree &HT,int a,int &s1,int &s2);
HuffmanTree Create(Code f[],int n){
	int m=2*n-1;
	HuffmanTree HT =new HTNode[m+1];
	int s1,s2;
	for(int i=1;i<=m;i++){
		HT[i].lchild=0,HT[i].rchild=0,HT[i].parent=0,HT[i].weight=0;
	}
	
	
	int head=1;
	for(int i=0;i<256;i++){
		if(f[i].cnt>0){
			HT[head].sign=f[i].name;
			HT[head++].weight=f[i].cnt;
		} 
	}
	
	for(int i=n+1;i<=m;i++){
		Select(HT,i-1,s1,s2);
		HT[s1].parent = i,HT[s2].parent=i;
		HT[i].lchild = s1;HT[i].rchild=s2;
		HT[i].weight = HT[s1].weight + HT[s2].weight;
	}
	return HT;
}

void Select(HuffmanTree &HT,int a,int &s1,int &s2){
	int i,j,x,y;
	for(j=1;j<=a;j++){
		if(HT[j].parent==0){
			x=j;
			break;
		}
	}
	for(i=1;i<=a;i++){
		if(HT[i].weight<HT[x].weight&&HT[i].parent==0){
			x=i;
		}
	}
	for(j=1;j<=a;j++){
		if(HT[j].parent==0&&j!=x){
			y=j;
			break;
		}
	}
	for(i=1;i<=a;i++){
		if(HT[i].weight<HT[y].weight&&HT[i].parent==0&&i!=x){
			y=i;
		}
	}
	s1=x,s2=y;
}

typedef char ** HuffmanCode;
void CrtHuffmanCode1(HuffmanTree ht,HuffmanCode hc,int n){
	char *cd;
	int start;
	cd =(char *)malloc(sizeof(char)*n);
	cd[n-1]='\0';
	
	for(int i=1;i<=n;i++){
		int start=n-1;
		int c=i;
		int p=ht[i].parent;
		while(p!=0){
			--start;
			if(c==ht[p].lchild)
				cd[start]='0';
			else if(c==ht[p].rchild)
				cd[start]='1';
			
			c=p;
			p=ht[p].parent;				
		}
		hc[i]=(char*)malloc( (n-start) * sizeof(char) );
		strcpy(hc[i],&cd[start]); 
	}
	free(cd);
}


// ---------- 将二进制码 还原成源文件  ----------------
void Decode(string s,HuffmanTree ht,int n,int tail,char *filename){
    string s1(filename);
    string s2(".decode");
    string s3=s1+s2;
    FILE *fout;
    if((fout=fopen(s3.c_str(),"wb"))==NULL){
        printf("write decode error\n");
    }

	int j=2*n-1;
	int i=0;
	for(int i=0;i<s.length()-(8-tail);i++)
	{
		if(s[i]=='0')
			j=ht[j].lchild;
		else 
			j=ht[j].rchild;
		if(ht[j].lchild==0){
            fputc(ht[j].sign,fout);
			j=2*n-1;
		}
	}
    fclose(fout);
} 


bool cmp(Code a,Code b){
	return a.cnt>b.cnt;
}

Code f[256];


int compress(string str,char *filename){
    
    string s1(filename);
    string s2(".code");
    string s3=s1 + s2;
	FILE *ostream;
    //cout<<s3<<endl;
    if((ostream=fopen(s3.c_str(),"ab+"))==NULL){
        cout<<"read code error"<<endl;
    }
    fseek(ostream,0,SEEK_END);
    int cnt=0;
    int sum=0;

    //string str="0111111101";
    int tail=str.length()%8;

    putw(tail,ostream);
    int end=str.length()-tail;
    for(int i=0;i<end;i++){
        cnt++;
        sum <<=1;
        sum |=str[i]-'0';
        if(cnt %8==0  &&cnt){
            //cout<<sum<<endl;
            fputc(sum,ostream);
            sum=0;
        }    
    }
    //cout<<"sum="<<sum<<endl;
    //sum=0;
    for(int i=end;i<str.length();i++){
        if(str[i]=='1'){
            int ret=1;
            ret <<=(7-(i-end));
            sum +=ret;
        }
    }
    //cout<<sum<<endl;
    fputc(sum,ostream);
    fclose(ostream);
	return tail;
}

string depress(char *filename){
    string s1(filename);
    string s2(".code");
    string s3=s1+s2;
    FILE *fp;
	if((fp=fopen(s3.c_str(),"rb"))==NULL){
        printf("fopen code error");
    }
    int c;
	string str="";
    while((c=fgetc(fp))!=EOF){ 
        for(int i=7;i>=0;i--){
            int ret=1;
            ret <<=i;
            if(c&ret){
                //cout<<"1";
		        str+="1";
            }
            else{
                str+="0";
            }
        }
        
    }
    fclose(fp);
	return str;
}

int pd(char *argv){
    int len=strlen(argv);
    string tmp_s2(argv);
    int flag=0;
    int index;
    int sublen=0;
    for(int i=len-1;i>=0;i--){
        if(argv[i]=='.'){
            flag=1;
            index=i;
            break;
        }
        sublen++;
    }
    if(flag==1){
        tmp_s2=tmp_s2.substr(index+1,sublen);
        if(strcmp("png",tmp_s2.c_str())==0){
            //cout<<tmp_s2<<endl;
            return 1;
        }
        if(strcmp("jpg",tmp_s2.c_str())==0){
            //cout<<tmp_s2<<endl;
            return 2;
        }
        

    }
    return 0;
}
int main(int argc,char **argv)
{
    
    //freopen("out.txt","w",stdout);
    
    int filetype=pd(argv[1]);
	FILE *istream;
	if((istream=fopen(argv[1],"rb"))==NULL){
		cout<<"fopen argv[1] error"<<endl;
	}
    int ret[10];
    if(filetype==1){
        for(int i=0;i<8;i++){
            ret[i]=fgetc(istream);
        }
        //ret1=fgetc(istream);
        //ret2=fgetc(istream);
    }
    if(filetype==2){
        for(int i=0;i<6;i++){
            ret[i]=fgetc(istream);
        }
    }

    memset(f,0,sizeof(f));
    
    vector<int> all_string;
    int n=0;    //哈夫曼树中节点的个数 
   	int  c;
	while((c=getc(istream))!=EOF){
        f[c].name=c;
		++f[c].cnt;
        all_string.push_back(c); 
        //c=fgetc(istream);
        //fread((char*)&c,sizeof(unsigned char),1,istream); 
	}
    fclose(istream);

    //cout<<"hello world"<<endl; 
    /* ---------测验统计 ------------------*/
    //cout<<"sdsdas"<<endl;
    string tmp_s1=string(argv[1])+".code";
    FILE *ostream;
    if((ostream=fopen(tmp_s1.c_str(),"wb"))==NULL){
        printf("fopen out.tmp error\n");
    }

    if(filetype==1){
        for(int i=0;i<8;i++){
            fputc(ret[i],ostream);
        }
    }
    if(filetype==2){
        for(int i=0;i<6;i++){
            fputc(ret[i],ostream);
        }
    }

	for(int i=0;i<256;i++){
		if(f[i].cnt>0){
			n++;
            //cout<<i<<" "<<f[i].cnt<<endl;
            //fwrite(&f[i],sizeof(f[i]),1,ostream);
		}
	}
    int ptr=n;
    //cout<<"ptr="<<ptr<<endl;
    putw(ptr,ostream);
    //fwrite(&ptr,sizeof(int),1,ostream);
	for(int i=0;i<256;i++){
		if(f[i].cnt>0){
			//n++;
            //cout<<i<<" "<<f[i].cnt<<endl;
            fwrite(&f[i],sizeof(f[i]),1,ostream);
		}
	}
    fclose(ostream);
	
    
    
	HuffmanTree ht=Create(f,n);
	int m=2*n-1;
	
	HuffmanCode hc=(char**)malloc(sizeof(char*)*(n+1));
	for(int i=1;i<=n;i++)
		hc[i]=(char*)malloc(sizeof(char)*n);
    

    // ------创建哈弗曼树 --------
	CrtHuffmanCode1(ht,hc,n);
    /*
    for(int i=1;i<=n;i++){
        cout<<ht[i].sign<<"  "<<hc[i]<<endl;
    }
    */
	//cout<<"all_string="<<all_string<<endl;
    // -------------将二进制码写入code 文件-------------
    	
    string tmp_string="";
    for(vector<int>::iterator  i=all_string.begin();i!=all_string.end();i++){
		for(int j=1;j<=n;j++){
			if(ht[j].sign==*i){
                tmp_string +=hc[j];
                break;
			}
		}
	}

    //cout<<"tmp_string="<<tmp_string<<endl;
    //char ttt[]="in2";
	int tail=compress(tmp_string,argv[1]);
	
    
    /*
	// -----------读取 code 文件  还原原文件------
	
	string s2=depress(argv[1]);
	Decode(s2,ht,n,tail,argv[1]);
    */

	return 0;
}
