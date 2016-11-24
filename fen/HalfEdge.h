
class Solid;
class Face;
class Loop;
class Edge;
class HalfEdge;
class Vertex;
class Point;

//体
class Solid
{
public:

	Solid():prevs(NULL), nexts(NULL), sFace(NULL), edge(NULL){}

	Solid *prevs;
	Solid *nexts;
	Face *sFace;	//首面;
	Edge *edge;		//用于线框显示的边
};

//面
class Face
{
public:

	Face():prevf(NULL), nextf(NULL), fSolid(NULL), fLoop(NULL){}

	Face *prevf;
	Face *nextf;
	Solid *fSolid;
	Loop *fLoop;	//首环,内环

};

//环
class Loop
{
public:

	Loop():prevl(NULL), nextl(NULL), lFace(NULL), lEdge(NULL){}

	Loop *prevl;
	Loop *nextl;//注意，loop的前后Loop指的一般同一face的。
	Face *lFace;
	HalfEdge *lEdge;//首边
};

//半边
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
	Edge *edge;    //边
};



//边
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

//顶点
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

	//欧拉操作
	Solid * mvfs(Point point, Vertex *& newVertex);	//构造一个体,一个面,一个外环,一个点
	HalfEdge * mev(Vertex *v1, Point p2, Loop *lp ); //构造一个新点,同时构造一条连接新点与一给定点的边
	Loop * mef(Vertex *v1, Vertex *v2, Loop *lp);//构造一个边,一个面,一个环
	Loop * kemr(Vertex *v1, Vertex *v2, Loop *lp);//删除一条边构造一个环
	void kfmrh(Loop *outlp, Loop *lp);//删除一个与面f1相接触的一个面f2，形成体上一个通孔
	
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