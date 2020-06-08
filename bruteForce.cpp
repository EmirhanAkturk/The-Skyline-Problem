/* 
    Algoritma Analizi programlama ödevi 1 
    Emirhan Aktürk 
    19120205058
    
    x eksenindeki noktalar için maksimum yüksekliği bulup diziye kaydeden algoritma.
    Bu algoritmada brute force mantığı kullanılmıştır.
    
    x eksenindeki noktalar için 
    (tüm şeklin yatay uzunluğu = m)
    (dörtgen sayisi=row=n)
    getRow:O(n) 
    fread:O(n)
    printLines:O(n)
    updateHeight:O(m.n^2)      eğer m~n ise O(n^3)
    getLength:O(m)
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

static int maxEnd;
static int minStart=1000;//max deger ile ilklendirmek için


/* 
getRow:while döngüsü dosyadaki karakter sayısı kadar dönecek.
Karakter sayısı=row*(c sabiti)  (row=n)
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

/* printLines:lines.size() = row .Çalışma süresi O(n) */
void printLines(const vector<Line>&lines){
    for(int i=0;i<lines.size();++i){
        cout<<i+1<<".line:("<<lines[i].p1.x<<","<<lines[i].p1.y<<"),"<<
        "("<<lines[i].p2.x<<","<<lines[i].p2.y<<")"<<endl; 
    }
}

/* 
updateHeights: heights dizisini dolaşarak her x değeri için max yüksekliği tabloya kaydeden fonksiyon.
En dıştaki for heights dizisini bir tur dolaşacak.
ortadaki for her x kordinat değeri için yatay doğrulardan birinin içinde mi diye kontrol ediyor
en içteki for bulunan yatay doğrunun her noktası ile heights dizisindeki yükseklikleri karşılaştırıyor ve gerekirse değeri güncelliyor

son durumda ortaya çıkan şeklin yatay uzunluğu = heights.size() = m  // xLines.size()=row=n olduğuna göre
fonksiyonun çalışma zamanı:O(m.n^2) 
n~m --> O(n^3)
*/
void updateHeights(vector<int>&heights,const vector<Line>&xLines){
    for(int i=0;i<heights.size();++i){
        for(int j=0;j <xLines.size();++j){
            if( (i+minStart)>= xLines[j].p1.x && (i+minStart)<= xLines[j].p2.x){
                for(int k=xLines[j].p1.x; k<=xLines[j].p2.x ;++k){
                    if(heights[k-minStart]<xLines[j].p1.y)
                        heights[k-minStart]=xLines[j].p1.y;
                }
            }
        } 
    }
}

/* 
getLength:heights dizisini bir tur geziyor 
son durumda ortaya çıkan şeklin yatay uzunluğu = heights.size() = m olduğuna göre
fonksiyonun çalışma zamanı:O(m)
*/
int getLength(const vector<int>&heights){
    int length=heights[0];
    for(int i=1;i<heights.size();++i){
        length+=abs(heights[i]-heights[i-1])+1;
    }

    length+=abs(heights[heights.size()]-heights[heights.size()-1]);
    return length;
}

int main(){
    int row=getRow();

    vector<Line>xLines(row);
    
    fread(xLines);
    vector<int>heights(maxEnd-minStart+1);
        
    cout<<"xLines:"<<endl;
    printLines(xLines);

    updateHeights(heights,xLines);
    
    for(int a:heights)
        cout<<a<<" ";


    cout<<"\nlength:"<<getLength(heights)<<endl;
    

    return 0;
}