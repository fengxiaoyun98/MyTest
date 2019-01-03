//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// DoubleXY.cpp: implementation of the CDoubleXY class.
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "stdafx.h"
#include "DoubleXY.h"
#include "math.h"
#include "float.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CDoubleXY::MultiXYToY(void)
{
	CDoubleArray::iterator itx;
	CDoubleArray::iterator ity;
	itx=x.begin();
	ity=y.begin();
	for(;itx<x.end();itx++,ity++)
		*ity=*ity**itx;
}

//double����ж�
bool dbIsEqu(double a,double b){return fabs(a-b)<DBL_EPSILON;}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CDoubleXY::CDoubleXY(void)
{
	ldata=0;
}
CDoubleXY::CDoubleXY(UINT nNewSize)
{
	ReSize(	nNewSize);
	ldata=0;
//	test();
}

void CDoubleXY::test(void)
{
	double x0,x1,x2,x3;
	x0=pow(2,(double)0);
	x0=23,x1=1.5,x2=0.8,x3=3;
	UINT cnt=20;
	UINT power=3;
	CDoubleArray dx(cnt),dy(cnt),scale(10);
	for(UINT i=0;i<cnt;i++){
		double v;
		v=dx[i]=i*0.4;
		dy[i]=x3*pow(v,3)+x2*pow(v,2)+v*x1+x0;
	}

	ReSize(cnt);
	x.assign(dx.begin(),dx.end());
	y.assign(dy.begin(),dy.end());
	ToNPowXY(3);

#define ��С������ϲ���
#if defined(��С������ϲ���)
	CalculateCurveParameter(&dx,&dy,power+1,cnt,&scale);
	if(dbIsEqu(x0,scale[0]))
		return;
#elif defined(���η�����ֵ)
#endif
}
//range
//��������ϵĸ���X��Yֵ
//rge���ε㷶Χ���Ը�����Ϊ����
//��ϴ����ϵ�<10
double CDoubleXY::GetPowYAtX(UINT power,double atx,UINT rge)
{
	double aty=atx;
	if(rge==0)
		rge=20;
	UINT pos=GetYAtX(aty);//���λ��
	if(!(pos<ldata||pos>0))
		return 0;
	UINT s,e;
	if(ldata<rge)
		rge=ldata;
	if(ldata<pos+rge/2){
		s=0;e=ldata;
	}else{
		if(pos<rge/2){
			s=0;e=rge;
		}else{
			s=pos-rge/2;e=pos+rge/2;
		}
	}
	if(rge!=e-s)//�����ھ���������
		return 0;
	CDoubleArray scale(power+1);
	CDoubleArray ix,iy;
	ix.assign(x.begin()+s,x.begin()+e);
	iy.assign(y.begin()+s,y.begin()+e);
	CalculateCurveParameter(&ix,&iy,power,rge,&scale);
	aty=0;
	for(UINT p=0;p<=power;p++)
		aty+=pow(atx,(double)p)*scale[p];
	return aty;
}
//range
//��������ϵĸ���X��Yֵ
//rge���ε㷶Χ���Ը�����Ϊ����
//����������Ч��IV�����¹���
void CDoubleXY::GetPowMax(double &v,double &i,double &pm,UINT rge)
{
	//������������ʵ�
	//������Ϻ�����ϵĺ�����������ʵ�
	v=i=pm=0;
	if(rge==0)//Ĭ������ʵ㸽��20����
		rge=20;

	UINT pos=0;
	UINT power=5;//ͳһ��ϴ���
	double Pmax=GetMaxXY(pos);//����ʵ���λ��
    
/*	CString str;
	str.Format(L"����ʵ�:pos:%d \n Pmax:%f \n",pos, Pmax);
	AfxMessageBox(str);
*/
	if(!(pos<ldata||pos>0))
		return ;
	double aty,atx;
	aty=y[pos],atx=x[pos];
	UINT s,e;
	if(ldata<rge)
		rge=ldata;
	if(ldata<pos+rge/2){
		s=0;e=ldata;
	}else{
		if(pos<rge/2){
			s=0;e=rge;
		}else{
			s=pos-rge/2;e=pos+rge/2;
		}
	}
	if(rge!=e-s)//�����ھ���������
	{
		return;
	}
	CDoubleArray scale(power+1);
	CDoubleArray ix,iy;
	ix.assign(x.begin()+s,x.begin()+e);
	iy.assign(y.begin()+s,y.begin()+e);
	CalculateCurveParameter(&ix,&iy,power,rge,&scale);
	//��һ����������������ʵ�
	//��αƽ�
	double max=DBL_MIN;
	double min=DBL_MAX;
	CDoubleArray::iterator it;
	for(it=ix.begin();it<ix.end();it++){
		if(*it<min)min=*it;
		if(*it>max)max=*it;
	}

	double lmt0,lmt1;//��Ч������ֵֹ
 
/*
	str.Format(L"��Ч������ֵֹ:min:%f \n max:%f \n",min, max);
	AfxMessageBox(str);
*/
	lmt0=min,lmt1=max;
	aty=0;
	//��ʼֵ
	atx=(max+min)/2;
	double rl=(max-min)/2;//��ֵ���䳤��
	for(UINT p=0;p<=power;p++)
		aty+=pow(atx,(double)p)*scale[p];
	max=aty*atx;
	double ri,rv;
	ri=aty,rv=atx;
	int dir=1;//����
	int lmt=100;//�����������
	int cnt=0;

	double pre;//ǰһ�ν��
	pre=max;
	//�۰�����
	do{
		atx+=rl*dir;
		if(atx>lmt1)
			atx=lmt1;
		if(atx<lmt0)
			atx=lmt0;
		aty=0;
		for(UINT p=0;p<=power;p++)
			aty+=pow(atx,(double)p)*scale[p];
		min=atx*aty;

		//�����������ﵽҪ��
		if(fabs(max-min)<0.00000001){
			if(cnt>2){//���������޸Ľ�
				i=ri;v=rv;pm=i*v; //�Ķ�      i=ri;v=rv;pm=max;
/*
				str.Format(L"���������޸Ľ�,����������pm:%f \n i:%f \n v:%f \n",pm, i, v);
				AfxMessageBox(str);
*/
				return;
			}else cnt++;
		}
		int isdir=dir;
		if(max<min){
			cnt=0;
			ri=aty,rv=atx;
			max=min;
		}
		if(pre>min){//��֤��������
			dir<0?dir=1:dir=-1;
		}
		if(dir!=isdir)
			rl=rl*7/11;//��һ�η��򲽾��С
		pre=min;

		if(--lmt<0)//ʧ��
			break;
	}while(1);
	//�Ķ�
    //pm=Pmax,i=y[pos],v=x[pos]; 
	i=y[pos],v=x[pos]; 
	pm = i*v;
	return;
}

//��һ�β�����ת����N(power)�����߶�
//ȫ�����̰�ת���ɹ�����û��ʧ�ܵĴ������
void CDoubleXY::ToNPowXY(UINT power,UINT p1,UINT p2)
{
	if(power<1||power>100||(p1==p2&&p1!=0)||ldata==0)
		return;
	if(p1>p2){
		UINT t=p2;p2=p1;p1=t;
	}
	if(p1>ldata)
		return;
	if(p1==0)//defalut
		p2=ldata;
	if(p2==0||p2>ldata)
		p2=ldata;
	UINT cnt=p2-p1;
	CDoubleArray scale(power+1);
	CalculateCurveParameter(&x,&y,power,cnt,&scale);
	for(UINT i=0;i<cnt;i++){
		y[i]=0;
		for(UINT p=0;p<=power;p++)
			y[i]+=pow(x[i],(double)p)*scale[p];
	}
}
//����X��Y��ֵ
double CDoubleXY::EvaluateY(double thex)
{
	//����1
	return GetPowYAtX(5,thex);
	//����2
	double val=thex;
	UINT pos=0;
	pos=GetYAtX(val);
	if(pos<ldata)
		return val;
	return 0;
}
double CDoubleXY::EvaluateX(double they)
{
	double val=they;
	UINT pos=0;
	pos=GetXAtY(val);
	if(pos<ldata)
		return val;
	return 0;
}

CString CDoubleXY::GetFileName(BOOL o)
{
	TCHAR sCurrentDirectory[MAX_PATH];
	GetModuleFileName(AfxGetInstanceHandle(),sCurrentDirectory,sizeof(sCurrentDirectory));
	for(UINT i=_tcslen(sCurrentDirectory)-1;i>0;i--)
	{
		if(sCurrentDirectory[i] == 92)
		{
			sCurrentDirectory[i] = 0;
			break;
		}
	}
	CString str=_T("(I,V) List(*.CSV)|*.CSV");
	CString str1=_T("CSV");

	CFileDialog of(TRUE,str1,NULL,NULL,str);
	CFileDialog sf(FALSE,str1,NULL,NULL,str);
	CFileDialog *p;
	if(o)
		p=&sf;
	else
		p=&of;
	
	p->m_ofn.lpstrInitialDir=sCurrentDirectory;

	if(p->DoModal()==IDOK )
	{
		str=p->m_ofn.lpstrFile ;
	}
	else str.Empty ();
	return str;
}


BOOL CDoubleXY::DoSeries(CString fn,BOOL o)
{
	if(fn.IsEmpty ())
		fn=GetFileName(o);
	if(fn.IsEmpty ())
		return false;
	
	CFile myFile;
	if(o){
		if(!myFile.Open (fn, CFile::modeNoTruncate| CFile::modeReadWrite)){
		MessageBox(0,_T("Open File error!\n\r")+fn+_T("\n\rPlease check or create it"),0,IDOK);
		return false;
		}
	}
	else if(!myFile.Open (fn, CFile::modeCreate |CFile::modeNoTruncate| CFile::modeReadWrite)){
		MessageBox(0,_T("Save File error!\n\r")+fn+_T("Please check your file system"),0,IDOK);
		return false;
	}

	myFile.SeekToBegin();
	std::vector<double>::iterator ix,iy;
	ix=x.begin();iy=y.begin();
	CString str;	
	if(o){
		CArchive arStore(&myFile, CArchive::store);
		for(;ix<x.end();){
			str.Format(_T("%f,%f\n"),ix,iy);
			arStore<<str;
			ix++;iy++;
		}
		arStore.Close();
	}else{
		if(!GetFileSize(myFile,NULL)){
			MessageBox(0,_T("File is empty!"),0,IDOK);
			return false;
		}
		
		CArchive arLoad(&myFile, CArchive::load);
		arLoad>>str;

		arLoad.Close();
	}
	myFile.Close ();
	return true;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CDoubleXY::MUT(UINT i,double b)
{
	x[i] *= b;
	y[i] *= b;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CDoubleXY::EquB(CDoubleXY *b)
{//ok
	ldata=b->GetDataSize();
	x.assign(b->x.begin(),b->x.begin()+ldata);
	y.assign(b->y.begin(),b->y.begin()+ldata);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CDoubleXY::AND(CDoubleXY *b,UINT ai,UINT bi)
{
	x[ai] += b->x[bi];
	y[ai] += b->y[bi];
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CDoubleXY::EquB(double ix,double iy,UINT i)
{
	x[i]=ix;	y[i]=iy;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CDoubleXY::EquB(CDoubleXY *b,UINT n)
{
	x[n]=b->x[n];
	y[n]=b->y[n];
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void CDoubleXY::EquB(CDoubleXY *b,UINT an,UINT bn,UINT n)
{//ok
	if(an<0||bn<0||n<0)
	{
		AfxMessageBox(_T("Label error"));
		return;
	}
	if ((an+n) == size())      //2017-4-8  mark
		return;
	if((an+n)>size())
		ReSize(an+n+10,size());
	x[an]=b->x[bn];
	y[an]=b->y[bn];
	for(UINT i=1;i<n;i++)
	{
		x[an+i]=b->x[bn+i];
		y[an+i]=b->y[bn+i];
	}
	ldata=an+n;
#ifdef _DEBUG
	//����ڴ�й©,Խ���
	//_ASSERTE(_CrtCheckMemory()); 
#endif
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CDoubleXY::~CDoubleXY()
{
	ReSize(0);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Bezier���ߵĵݹ��㷨
//rΪ���ɵ㼯��nΪ����Ч�ڵ�����sΪ��㣬����s<n,n-s>=2
//�ɼ������������mn+1,rnΪҪ����ĵ���
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
BOOL CDoubleXY::Bezier(CDoubleXY *r,UINT n,UINT s,UINT mn,UINT rn)
{
	if(n==0)n=size();
	if(n-s<2)return false;

	UINT k,j,i,h,g;

	g=n-s;
	CDoubleXY *t=new CDoubleXY(g+1);

	//���Լ���mn+1�������ԭ����β��
	if(rn==0)rn=mn;

	for(j=0;j<=rn;j++)
	{
		for(h=0;h<g;h++)
			t->EquB(this,h,s+h);
		double mut=j;
		mut /=mn;
		for(k=0;k<g-1;k++)
			for(i=0;i<g-1-k;i++)
				t->ForBezierMut(i,mut);
		r->EquB(t,j,0);
	}

	delete t;
	return true;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Bezier���ߵ����ƽ���㷨���㷨�а����˲�����
//�νӷ�ʽΪ�ص���ʽ�����Ϊ(UINT)times/2+1
//times�Ǵ�����Ҳ�Ƕε���,�����ܵ���r->l,r->l/n����Ϊ����
//rΪ���ɵ㼯��nΪ����Ч�ڵ���
//ȡ�㷽ʽ����һ��Ϊ��ǰ�ߵ�times��������times��ȡ��Ϊ
//��һ��Ϊǰһ����������м�һ����times/2+1����Ϊ������
//������Ϊ��һ��ĵ�times/2+1����λ���Ժ�times����
//ÿ�μ�����ֻ����times/2+1�����������һ����Ϊ�νӵ�
//���һ����ȫ������
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CDoubleXY::BezierContinue(CDoubleXY *r, UINT n, UINT times, UINT inpd)
{
	//inpd�ڵ�֮��������
	if(times<3||inpd<3)return ;
	//���������
	UINT rall=0;
	CDoubleXY *t=new CDoubleXY(inpd*(times-1)+1);//һ�μ������㼯
	CDoubleXY *p=new CDoubleXY(times);//һ�μ�����ʱ�㼯
	//��һ���
	UINT k=1;
	p->EquB(this,0);
	do
	{
		//ÿ�μ��������������ڵ��Ĳ�����
		//���ּ���������һ�������һ���ӵ�
		if(k+times-1<n)
		{
			p->EquB(this,1,k,times-1);
			p->Bezier(t,times,0,inpd*(times-1),inpd+1);
			r->EquB(t,(k-1)*inpd,0,inpd+1);
			p->EquB(t,0,inpd);
		}
		else
		{
			rall=inpd*k;
			break;
		}
		k++;
	}while(1);
	delete t;
	delete p;
	r->SetDataSize(rall);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Bezier���ߵķֶ�ƽ���㷨���㷨�а����˲�����
//�νӷ�ʽΪ�ص���ʽ�����Ϊ(UINT)times/2+1
//times�Ǵ�����Ҳ�Ƕε���,�����ܵ���r->l,r->l/n����Ϊ����
//rΪ���ɵ㼯��nΪ����Ч�ڵ���
//ȡ�㷽ʽ����һ��Ϊ��ǰ�ߵ�times��������times��ȡ��Ϊ
//��һ��Ϊǰһ����������м�һ����times/2+1����Ϊ������
//������Ϊ��һ��ĵ�times/2+1����λ���Ժ�times����
//ÿ�μ�����ֻ����times/2+1�����������һ����Ϊ�νӵ�
//���һ����ȫ������
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
UINT CDoubleXY::BPlus(CDoubleXY *r,UINT n,UINT times,UINT inpd)
{
	//inpd�ڵ�֮��������
	if(times<3||inpd<3)return 0;
	
	//�ܽ������=(n-1)*inpd
	UINT	in=(times-1)*inpd;//һ�μ������ܵ���
	//ÿ�μ��㱣������
	//times*l/n/2Ϊ�����м�λ��
	UINT ij=in/2;
	//Times����ż�ж�
	UINT tt;
	if(times%2)tt=1;else tt=0;
	//�м��ص�������������λ��nt*k+1;
	UINT nt=times/2;
	//ʣ�����
	UINT sd;
	//���������
	UINT rall=0;
	CDoubleXY *t=new CDoubleXY(in+1);//һ�μ������㼯
	CDoubleXY *p=new CDoubleXY(times);//һ�μ�����ʱ�㼯
	//��һ���
	UINT k=0;
	do
	{
		sd=k*nt+tt+times-n;
		//���ܵ������ε������Դ�������Ĺ�ϵ�ж�ѡ�������
		if(sd<=0)//ʣ�������һ��
		{
			//��֯һ�ε�
			if(k!=0)
			{
				p->EquB(t,0,ij-1);//��һ�����Ľ�����м��
				p->EquB(this,1,k*nt+tt,times-1);
			}
			else p->EquB(this,0,0,times);
			
			if(sd==0)//ʣ��������ù�һ��
			{

				p->Bezier(t,times,0,in);
				r->EquB(t,k*ij-1,0,in+1);
				rall=k*ij-1+in+1;
				break;
			}
			else
			{
				p->Bezier(t,times,0,in,ij+2);
				if(k==0)
					r->EquB(t,0,0,ij+1);
				else
					r->EquB(t,k*ij-1,0,ij+1);//һ���غ�
			}
		}
		else if(sd>0)
		{
			if(k==0)//�ܵ�������һ��
			{
				rall=(n-1)*inpd;
				p->EquB(this,0,0,n);
				p->Bezier(t,n,0,rall);
				r->EquB(t,0,0,rall+1);
			}
			else//��ʣ�����������һ�Σ�������֯���Ϊһ��
			{
				sd=n-(k*nt+tt);//���β��������������������sd<(nt+tt)
				p->EquB(t,0,ij-1);//��һ�����Ľ�����м��				
				p->EquB(this,1,k*nt+tt,sd);//��һ�������ص���������
				sd++;//�ܵ���=�м��+�ص���+�����
				UINT sr=(sd-1)*inpd;//���һ�ν������
				t->ReSize(sr+1);//���ý��������
				p->Bezier(t,sd,0,sr);
				r->EquB(t,k*ij-1,0,sr+1);
				rall=k*ij-1+sr+1;
			}
			break;
		}
		k++;
	}
	while(1);
	delete t;
	delete p;
	r->SetDataSize(rall);
	return rall;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void CDoubleXY::ReSize(UINT nNewSize, UINT rSize)
{
	if(nNewSize < 0)return;
	x.resize (nNewSize);
	y.resize (nNewSize);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//len����Ч���ȴ�0��ʼ
//pos�������ֵλ��
//����ֵΪ���ֵx*y
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
double CDoubleXY::GetMaxXY(UINT &pos, UINT len)
{
	UINT n;
	if(len==0)
		n=ldata;
	else 
		n=len;
	double tpMax=DBL_MIN;
	for(UINT i=0;i<n;i++)
	{
		double tp;
		tp=x[i]*y[i];
		if(tp>tpMax)
		{
			tpMax=tp;
			pos=i;
		}
	}
	return tpMax;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//��С���˷�ֱ�����
//����б�ʣ�rs��rsh��������isc��voc
//kΪϵ����bΪ�ؾ࣬
//nΪҪ������Ч������sΪ���λ��
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

BOOL CDoubleXY::TransToLine(double &k, double &b, UINT n, UINT s)
{
	//���ֱ�߷���(Y=kX+b)
	if(n==0)
		n=x.capacity();
    if(n<2||s+n>size())
		return FALSE; 
	
	double *lx,*ly;
	lx=&x[s],ly=&y[s];

    double mX,mY,mXX,mXY; 
	mX=mY=mXX=mXY=0; 
	for(UINT i=0;i<n;i++)
	{ 
		mX+=lx[i]; 
		mY+=ly[i]; 
		mXX+=lx[i]*lx[i]; 
		mXY+=ly[i]*lx[i]; 
	}
	if(dbIsEqu(mX*mX,mXX*n))
		return FALSE; 
	k=(mY*mX-mXY*n)/(mX*mX-mXX*n); 
	b=(mY-mX*k)/n; 
	return TRUE; 
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

double CDoubleXY::GetX(UINT n)
{
	if(n<size())
		return x[n];
	else 
		return 0.0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

double CDoubleXY::GetY(UINT n)
{
	if(n<size())
		return y[n];
	else 
		return 0.0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void CDoubleXY::SetX(double vx,UINT n)
{
	if(n<size())
		x[n]=vx;
}
void CDoubleXY::SetX(double base,double step,UINT s,UINT n)
{
	if(n>size()||n==0)
		n=size();
	if(s>n)return;

	int i,t=0;
	for(i=s;i<n;i++){
		x[i]=base+step*t;
		t++;
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CDoubleXY::SetY(double vy,UINT n)
{
 	if(n<size())
		y[n]=vy;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//nΪ������
//sΪ������

void CDoubleXY::SetZero(void)
{
	x.clear();
	y.clear();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//x��valֵ����λ�õ�yֵ,val����yֵ����������λ��
UINT CDoubleXY::GetYAtX(double &val)
{
	UINT pos=0;
	double min0,temp;
	min0=DBL_MAX;
	for(UINT i=0;i<ldata;i++)
	{
		temp=fabs(val-x[i]);
		if(min0>temp)
		{
			min0=temp;
			pos=i;
		}
	}
	val=y[pos];
	return pos;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//y��valֵ����λ�õ�xֵ,val����xֵ����������λ��
UINT CDoubleXY::GetXAtY(double &val)
{
	UINT pos=0;
	double min0,temp;
	min0=DBL_MAX;
	for(UINT i=0;i<ldata;i++)
	{
		temp=fabs(val-y[i]);
		if(min0>temp)
		{
			min0=temp;
			pos=i;
		}
	}
	val=x[pos];
	return pos;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//����֮ǰ�����б�����Ա仯��һ��
//��һ�����ݰ������Է��̱任��k,b���̲���
//n��Ч������s���,ch=1��x���Ի�y��ch=0��y���Ի�x
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CDoubleXY::Liner(double k, double b, UINT n, UINT s,BOOL ch)
{
	if(n<2||s<0)return;
	double step;
	if(ch) //��Xֵ���Ի�Y
	{
		step=(x[s+n-1]-x[s])/(n-1);
		for(UINT i=0;i<n;i++)
		{
			x[s+i]=x[s]+step*i;
			y[s+i]=k*x[s+i]+b;//y=kx+b
		}
	}
	else //��Yֵ���Ի�X��Y������0
	{
		if(dbIsEqu(k,0))
			return;
		step=(-b/k-x[s])/(n-1); 
		for(UINT i=0;i<n;i++)         //mark �Ķ�
		{
			x[s+i]=step*i+x[s];
			y[s+i]=k*x[s+i]+b;//y=kx+b
		}
	}
}
//ȥ�ֲ�ë��
//ԭ��ǰ�����ݵ�Yֵ����仯����ë�̣�ʹ��ǰ����ƽ��ֵ����
//s��㣬n�յ�,d����1����0��С��
void CDoubleXY::SmoothY(UINT s,UINT n,BOOL d)
{
	if(s+2>n)
		return;
	for(UINT i=s;i<n;i++)
	{
		if(i+2>n)
			return;
		if (y[i] > -0.000001 && y[i] < 0.000001)   //mark y[i] == 0�Ļ� �޷��˳�ѭ��
		{
			return;
		}
		if(d?y[i+1]<y[i]:y[i+1]>y[i])
		{
			UINT t=i+1;
			for(;t<n;t++)
				if(d?y[t]>y[i]:y[t]<y[i])
				{
					break;
				}else
				{
					if(t==n)return;
				}
			y[i+1]=y[i]+(y[t]-y[i])/(t-i);
			if(i>s)i--;
		}
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CDoubleXY::ForBezierMut(UINT i, double mut)
{
	x[i] = (x[i+1]-x[i])*mut+x[i];
	y[i] = (y[i+1]-y[i])*mut+y[i];
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Bezier���ߵĵݹ��㷨,�ֶ�ƽ���㷨
//nΪ����Ч�ڵ�����times�γ���inpdΪ�ڵ�����ĵ�����rΪ����㼯
//�㷨��
//������ѡ���м�㣬����ε���ǰ�����ݣ��ɴ˵���Ժ�ĵ����һ
//�μ��㣬������ظ���������ָ������������еĵ㡣
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CDoubleXY::BezierCurve(CDoubleXY *r, UINT n, UINT times, UINT inpd)
{
	if(times<5||inpd<3)return ;//inpd�ڵ�֮��������
	
	if(n<times)return;
	
	UINT rall=0;//���������
	UINT off=times/2;//�м�λ��


	UINT snum=inpd*times;//ÿ�β��������
	UINT mpos;//�м��λ��

	CDoubleXY *temp=new CDoubleXY(snum+1);//һ�μ�����ʱ�㼯

	UINT rldata =0;
	UINT ps,i;
	double min,mt;
	UINT k;
	for(k=0;(k+1)*off<ldata-times;k++)
	{
		min=10;
		ps=k*off;
		Bezier(temp,times+ps,ps,snum);
		for(i=0;i<snum;i++)//Ѱ����ӽ��м���λ��
		{
			mt=fabs(x[ps+off]-temp->x[i]);
			if(min>mt)
			{
				min=mt;
				mpos=i;
			}
		}
		x[ps+off]=temp->x[mpos];
		y[ps+off]=temp->y[mpos];
		r->EquB(temp,rldata,0,mpos);
		rldata +=mpos;
	}
	//�������һ���
	min=10;
	ps=k*off;
	snum=inpd*(ldata-ps);
	temp->ReSize (snum+1);
	Bezier(temp,ldata,ps,snum);
	r->EquB(temp,rldata,0,snum);

	delete temp;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//��Ч��Χ�����ݿ��ƶ�
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CDoubleXY::MoveBlock(UINT ds, UINT ss,UINT len)
{
	void *d,*s;
	if(len==0)
		len = size()-ss;
	len *=sizeof(double);
	d=&x[ds],s=&x[ss];
	memmove(d,s,len);
	d=&y[ds],s=&y[ss];
	memmove(d,s,len);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//���iv�ɼ����ݵ��¶����������ݴ���
//��xֵ���㴦����������Ҫ���ӻ����Ƴ�����
//������ǰ�ν������Ա任����������
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CDoubleXY::ReturnZeroX()
{
	UINT i;
	if(x[0]<0)
		for(i=0;i<size();i++) //ƫ�򸺰���
			if(x[i]>=0)
			{
				x[i]=0;
				MoveBlock(0,i);
				SetDataSize(size()-i);  
				break;
			}

	if(x[0]>0)
	{
		double k,b;
		TransToLine(k,b,30);
		y[0]=b;
		x[0]=0;
	}
}

UINT CDoubleXY::MultiXAny(double scale)
{
	std::vector<double>::iterator iter;
	iter=x.begin();
	UINT i=0;
	for(;iter<x.end();iter++){
		*iter*=scale;
		i++;
	}
	return i;

}
UINT CDoubleXY::MultiXAny(double Vscale,double Pscale,double TIsc,double TVoc)
{
	std::vector<double>::iterator iterx,itery;
	iterx=x.begin();
	itery=y.begin();
	UINT i=0;
	for(;iterx<x.end();iterx++,itery++)
	{
		if (*iterx == 0 && i > 300)
		{
			*iterx = 0;
		}else
			*iterx=(*iterx)*Vscale+(Pscale-1)*((*itery)+TIsc*((*iterx)/TVoc-1));    //����22�Ǿ����أ������ǷǾ���
		i++;
	}
	return i;
}
UINT CDoubleXY::MultiYAny(double scale)
{
	std::vector<double>::iterator iter;
	iter=y.begin();
	UINT i=0;
	for(;iter<y.end();iter++){
		*iter*=scale;
		i++;
	}
	return i;
}

double CDoubleXY::GetAverage(bool aix)
{
	std::vector<double>::iterator iter,itr;
	if(aix)
		itr=iter=x.begin();
	else
		itr=iter=y.begin();
	UINT i=0;
	double sum=0;
	UINT len=GetDataSize();
	for(;iter<itr+len;iter++){
		sum+=*iter;
		i++;
	}
	if(i==0)
		return -DBL_MAX;
	return sum/i;
}
double CDoubleXY::GetMax(bool aix)
{
	std::vector<double>::iterator iter,itr;
	if(aix)
		itr=iter=x.begin();
	else
		itr=iter=y.begin();
	UINT i=0;
	double max=-DBL_MAX;
	UINT len=GetDataSize();
	for(;iter<itr+len;iter++){
		if(max<*iter)
			max=*iter;
		i++;
	}
	if(i==0)
		return -DBL_MAX;
	return max;
}
double CDoubleXY::GetMin(bool aix)
{
	std::vector<double>::iterator iter,itr;
	if(aix)
		itr=iter=x.begin();
	else
		itr=iter=y.begin();
	UINT i=0;
	double min=DBL_MAX;
	UINT len=GetDataSize();
	for(;iter<itr+len;iter++){
		if(min>*iter)
			min=*iter;
		i++;
	}
	if(i==0)
		return DBL_MAX;
	return min;
}

//��С���˷��������
//���㷨����ax2+bx+c��M=3;������ͳ�ϵ��
BOOL CDoubleXY::CalculateCurveParameter(CDoubleArray *X,CDoubleArray *Y,UINT M,UINT N,CDoubleArray *A)
{
	//X,Y --  X,Y���������
	//M   --  �����������
	//N   --  ������Ŀ
	//A   --  �������
	register UINT i,j,k;
	double Z,D1,D2,C,P,G,Q;
	CDoubleArray B(N),T(N),S(N);
	if(M>N)M=N;
	for(i=0;i<M;i++)
		(*A)[i]=0;
	Z=0;
	B[0]=1;
	D1=N;
	P=0;
	C=0;
	for(i=0;i<N;i++){
		P=P+(*X)[i]-Z;
		C=C+(*Y)[i];
	}
	// 	C=(D1>0.0001)?(C/D1):0;    
	// 	P=(D1>0.0001)?(P/D1):0;
	C=C/D1;
	P=P/D1;
	(*A)[0]=C*B[0];
	if(M>1){
		T[1]=1;
		T[0]=-P;
		D2=0;
		C=0;
		G=0;
		for(i=0;i<N;i++){
			Q=(*X)[i]-Z-P;
			D2=D2+Q*Q;
			C=(*Y)[i]*Q+C;
			G=((*X)[i]-Z)*Q*Q+G;
		}
		// 		C=(D2>0.0001)?(C/D2):0;
		// 		P=(D2>0.0001)?(G/D2):0;
		// 		Q=(D1>0.0001)?(D2/D1):0;
		C=C/D2;
		P=G/D2;
		Q=D2/D1;
		D1=D2;
		(*A)[1]=C*T[1];
		(*A)[0]=C*T[0]+(*A)[0];
	}
	for(j=2;j<M;j++){
		S[j]=T[j-1];
		S[j-1]=-P*T[j-1]+T[j-2];
		if(j>=3){
			for(k=j-2;k>=1;k--)
				S[k]=-P*T[k]+T[k-1]-Q*B[k];
		}
		S[0]=-P*T[0]-Q*B[0];
		D2=0;
		C=0;
		G=0;
		for(i=0;i<N;i++){
			Q=S[j];
			for(k=j-1;;k--){
				Q=Q*((*X)[i]-Z)+S[k];
				if(k==0)
					break;
			}
			D2=D2+Q*Q;
			C=(*Y)[i]*Q+C;
			G=((*X)[i]-Z)*Q*Q+G;
		}
		// 		C=(D2>0.0001)?(C/D2):0;
		// 		P=(D2>0.0001)?(G/D2):0;
		// 		Q=(D1>0.0001)?(D2/D1):0;
		C=C/D2;
		P=G/D2;
		Q=D2/D1;
		D1=D2;
		(*A)[j]=C*S[j];
		T[j]=S[j];
		for(k=j-1;;k--){
			(*A)[k]=C*S[k]+(*A)[k];
			B[k]=T[k];
			T[k]=S[k];
			if(k==0)
				break;
		}
	}
	return TRUE;
}

void CDoubleXY::InitialXY()
{
	for (int i = 0; i < x.size(); i++)
	{
		x[i] = 0;
	}
	for (int i = 0; i < y.size(); i++)
	{
		y[i] = 0;
	}
}