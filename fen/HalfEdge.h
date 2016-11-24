
class Solid;
class Face;
class Loop;
class Edge;
class HalfEdge;
class Vertex;
class Point;

//��
class Solid
{
public:

	Solid():prevs(NULL), nexts(NULL), sFace(NULL), edge(NULL){}

	Solid *prevs;
	Solid *nexts;
	Face *sFace;	//����;
	Edge *edge;		//�����߿���ʾ�ı�
};

//��
class Face
{
public:

	Face():prevf(NULL), nextf(NULL), fSolid(NULL), fLoop(NULL){}

	Face *prevf;
	Face *nextf;
	Solid *fSolid;
	Loop *fLoop;	//�׻�,�ڻ�

};

//��
class Loop
{
public:

	Loop():prevl(NULL), nextl(NULL), lFace(NULL), lEdge(NULL){}

	Loop *prevl;
	Loop *nextl;//ע�⣬loop��ǰ��Loopָ��һ��ͬһface�ġ�
	Face *lFace;
	HalfEdge *lEdge;//�ױ�
};

//���
class HalfEdge
{
public:

	HalfEdge():prv(NULL), nxt(NULL), adjacentHe(NULL), startVe(NULL), endVe(NULL), hLoop(NULL), edge(NULL){}

	HalfEdge *prv;
	HalfEdge *nxt;
	HalfEdge *adjacentHe;
	Vertex *startVe;
	Vertex *endVe;
	Loop *hLoop;
	Edge *edge;    //��
};



//��
class Edge
{
public:
	
	Edge():preve(NULL), nexte(NULL), HalfEdgeL(NULL), HalfEdgeR(NULL){}
	
	Edge *preve;
	Edge *nexte;
	HalfEdge *HalfEdgeL;
	HalfEdge *HalfEdgeR;
};

class Point
{
public:

	double coord[3];
	double* GetCoord()
	{
		return coord;
	}
	void SetCoord(double x, double y, double z);
	void SetCoord(Point point);
	friend inline istream & operator >> (istream & is,Point & point)
	{
		is >> point.coord[0] >> point.coord[1] >> point.coord[2];
		return is ;
	}
	friend inline ostream& operator << (ostream & os,Point & point)
	{
		os << point.coord[0] << " " << point.coord[1] << " " << point.coord[2];
		return os ;
	}
};

//����
class Vertex
{
public:

	Vertex():prevv(NULL), nextv(NULL) {}

	Vertex *prevv;
	Vertex *nextv;
	Point point;
};



class Euler
{
public:

	Solid	*solid;

	//ŷ������
	Solid * mvfs(Point point, Vertex *& newVertex);	//����һ����,һ����,һ���⻷,һ����
	HalfEdge * mev(Vertex *v1, Point p2, Loop *lp ); //����һ���µ�,ͬʱ����һ�������µ���һ������ı�
	Loop * mef(Vertex *v1, Vertex *v2, Loop *lp);//����һ����,һ����,һ����
	Loop * kemr(Vertex *v1, Vertex *v2, Loop *lp);//ɾ��һ���߹���һ����
	void kfmrh(Loop *outlp, Loop *lp);//ɾ��һ������f1��Ӵ���һ����f2���γ�����һ��ͨ��
	
};

class MakeSolid
{
public:
	MakeSolid();
	~MakeSolid();
	Solid * CreateSweepSolid(string iPath, double vec[3], double d);
	Solid * GetSolid();
private:
	Euler *_Euler;
	void Sweep(Face *face, double vec[3], double d);
};