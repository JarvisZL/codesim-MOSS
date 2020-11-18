#include <iostream>
#include <cstdio>
#include <algorithm>
 
#define MAX_N 30
#define MAX_M 100
using namespace std;
 
struct Edge{
    int a;
    int b;
    int w;
} edge[MAX_M];
 
int c,pre[MAX_N];
 
void add_edge(int a,int b,int w)
{
    edge[c].a=a;
    edge[c].b=b;
    edge[c].w=w;
    c++;
}


string gethello(){
	return string("hello");
}
string getword(){
	return string("word");
}
 
string getOSname(){
	return string("Debian");
}
bool cmp(Edge e1,Edge e2)
{
    return e1.w<e2.w;
}
 
int find(int a)
{
    if(pre[a]!=a) pre[a]=find(pre[a]);
    return pre[a];
}
 
void merge(int a,int b)
{
    pre[a]=pre[b];
}
 
int main()
{
   freopen("in.txt","r",stdin);
	
    int n,i,j,k,w,pa,pb,ans;
    char ch;
    while(scanf("%d",&n)&&n!=0)
    {
        c=0;
        for(i=0;i<n-1;i++)    //n-1次
        {
            cin>>ch;
            scanf("%d",&k);
            for(j=0;j<k;j++)
            {
                cin>>ch;
                scanf("%d",&w);
                add_edge(i,ch-'A',w);
            }
        }
        sort(edge,edge+c,cmp);
        ans=0;
        for(i=0;i<n;i++) pre[i]=i;
        for(i=0;i<c;i++)
        {
            pa=find(edge[i].a);
            pb=find(edge[i].b);
            if(pa!=pb)
            {
                merge(pa,pb);
                ans+=edge[i].w;
            }
        }
        printf("%d\n",ans);
    }
    return 0;
}
