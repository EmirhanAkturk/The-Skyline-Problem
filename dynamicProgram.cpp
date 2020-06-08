/* 
    Algoritma Analizi programlama ödevi 1 
    Emirhan Aktürk 
    19120205058
    
    x ekseninde en baştan başlayarak önceki çözümleri belleğe kaydederek 
    önceki durum üzerine ekleme yapılıyor.
    Bu çözümde Dinamik programlama mantığı kullanılmıştır.

    (dörtgen sayisi=row=n)
    getRow:O(n) 
    fread:O(n)
    printLines:O(n)
    main:O(n^2)
    Total run time:O(n^2)
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

typedef struct{
    int x,y;
}point;

typedef struct{
    point p1;
    point p2;
}Line;

typedef struct{
    int height=0;
    int length=0;
}hl;
int  xLength,yLength;
static int maxEnd;
static int minStart=1000;//max deger ile ilklendirmek için

/* 
getRow:while döngüsü dosyadaki karakter sayısı kadar dönecek.
Karakter sayısı=row*(c sabiti)    (row=n)
Çalışma süresi O(n).
 */
int getRow(){
    ifstream ip("data.csv");
    if(!ip.is_open()) cout<<"ERROR:File Open"<<endl;
    char c;
    int row=1;
     while (ip.get(c)){
      if(c=='\n')
        ++row;
    }

    ip.close();
    return row;
}

/* 
sortLines:for döngüsü Lines.size() kadar,while döngüsü kendinden küçük eleman bulana kadar devam edecek.
Lines.size()<=2*row olduğuna göre row=n dersek
Average case = Worst case = O(n^2)
 */
void sortLines(vector<Line>&Lines){//insertion sort
    int i, j;  
    Line key;
    for (i = 1; i < Lines.size(); i++) 
    {  
        key = Lines[i];  
        j = i - 1;  

        while (j >= 0 && Lines[j].p1.x> key.p1.x) 
        {  
            Lines[j + 1] = Lines[j];  
            j = j - 1;  
        }  
        Lines[j + 1] = key;  
    }  
} 

/* 
fread:for döngüsü row kadar dönecek. row=n  dersek çalışma süresi:O(n)
 */
void fread(vector<Line> &xLines){
    int row=xLines.size();
    ifstream ip("data.csv");
    if(!ip.is_open()) cout<<"ERROR:File Open"<<endl;

    for(int i=0;i<row;++i){
        string height,weight,start;
        int h,w,s;
        int end;
        getline(ip,weight,';');
        getline(ip,height,';');
        getline(ip,start,'\n');
        w=stoi(weight);
        h=stoi(height);
        s=stoi(start);
        
        end=s+w;

        if(end>maxEnd)
            maxEnd=end;

        if(s<minStart)
            minStart=s;

        xLines[i].p1.x=s;
        xLines[i].p1.y=h;
        xLines[i].p2.x=s+w;
        xLines[i].p2.y=h;
    }
    

    ip.close();
}

/* printLines:lines.size() ~ 2*row .Çalışma süresi O(n) */
void printLines(const vector<Line>&lines){
    for(int i=0;i<lines.size();++i){
        cout<<i+1<<".line:("<<lines[i].p1.x<<","<<lines[i].p1.y<<"),"<<
        "("<<lines[i].p2.x<<","<<lines[i].p2.y<<")"<<endl; 
    }

}


/* dıştaki for xLine.size=row kadar dönüyor ve yatay doğrular arasında gezmeyi sağlıyor
içteki for yatay doğrunun üzerinde gezmeyi sağlıyor
(row=n) olduğuna göre
çalışma zamanı:O(n^2) */
int main(){
    int row=getRow();

    vector<Line>xLines(row);

    fread(xLines);
    sortLines(xLines);
    int size=maxEnd-minStart+1;
    vector<hl>v(size);
    
    cout<<"\nxLines:"<<endl;

    printLines(xLines);

    v[0].length=xLines[0].p1.y;
    v[0].height=xLines[0].p1.y;

    for(int i=0;i<xLines.size();++i){//i. yatay doğru
        for(int j=xLines[i].p1.x-minStart;j<=xLines[i].p2.x-minStart;++j){
            
            if(v[j-1].length!=0){
                if( xLines[i].p1.y>=v[j].height || xLines[i-1].p2.x==j){
                
                    if(j==xLines[i].p1.x){//j: x kordinatı
                        if(j==0){
                            v[j].length+=abs(xLines[i].p1.y-v[j].height);
                            v[j].height=xLines[i].p1.y;
                        }
                        else{
                            v[j].length=v[j-1].length+1+abs(xLines[i].p1.y-v[j].height);
                            v[j].height=xLines[i].p1.y;
                        }
                    }
                    
                    else if(j==xLines[i].p2.x){//j: x kordinatı
                        v[j].length=v[j-1].length+1+abs(xLines[i].p1.y-v[j].height);
                        v[j].height=xLines[i].p1.y;
                    }
                    
                    else{
                        v[j].length=v[j-1].length+1;
                        v[j].height=v[j-1].height;
                    } 
                
            }
        }
        else{
        
                int k=j-1;
                while(v[k].length==0)
                    --k;

                while(k<=j-1){
                    v[k].length=v[k-1].length+v[k].height+1;
                    ++k;}
                --j;
                
            }
        }
    }
    
    
    cout<<"\nHeight:";
    for(int i=0;i<size;i++)
        cout<<v[i].height<<" ";
    cout<<endl;
    cout<<"\nLenght:";
    for(int i=0;i<size;i++)
        cout<<v[i].length<<" ";
    cout<<endl;
    
    cout<<"\nlength of the rope:"<<v.back().length<<endl;
    return 0;
}