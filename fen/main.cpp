
#include "lib.h"
#include "Display.h"

int numOfPoint;
double vec[3] = { 0, 0, 1 };//sweep的方向
double d = 3;//sweep 长度

int numEdge(Loop *lp) //一个Loop里有多少个边
{
	HalfEdge *tHe = lp->lEdge;
	int numHe = 0;
	while (tHe)
	{
		numHe++;
		tHe = tHe->nxt;
		if (tHe == lp->lEdge)
			break;
	}
	return numHe;
}
bool equal(Vertex *v1, Vertex *v2)
{
	if (v1->point.coord[0] == v2->point.coord[0] &&
		v1->point.coord[1] == v2->point.coord[1] &&
		v1->point.coord[2] == v2->point.coord[2])
		return true;
	else
		return false;
}
bool equal(Loop *L1, Loop*L2)
{
	if (numEdge(L1) != numEdge(L2)) return false;
	Vertex *v1 = L1->lEdge->startVe;
	Vertex *v2 = L2->lEdge->startVe;
	int itera = numEdge(L1);
	while (itera--)
	{
		if (!equal(v1, v2))
			return false;
		v1 = L1->lEdge->nxt->startVe;
		v2 = L2->lEdge->nxt->startVe;
	}
	return true;
}
void OutBrp(Solid *Tsolid,string outPath)//生成brp文件
{
	ifstream infile;
	infile.open("inputfen.txt", ios::in);
	infile >> numOfPoint;
	infile.close();
	ofstream outfile;
	outfile.open(outPath.c_str(), ios::out);
	outfile << "BRP" << endl;
	Loop* loop_list[200];
	int loop_id = 0;
	Face *tface = Tsolid->sFace;
	int face_num = 0;
	int loop_num = 0;
	while (tface)
	{
		face_num++;
		Loop *tLoop = tface->fLoop;
		while (tLoop)
		{
			loop_num++;
			loop_list[loop_id++] = tLoop;
			tLoop = tLoop->nextl;
		}
		tface = tface->nextf;
	}
	outfile << numOfPoint*2 << " " << loop_num << " " << face_num << " " << 1 << endl;
	//列出Vertexs
	vector< Vertex*> VertexsList;
	Edge *Tedge = Tsolid->edge;
	while (Tedge != NULL)
	{
		bool IsExist = false;
		for each (Vertex *verc in VertexsList)
		{
			if (equal(verc, Tedge->HalfEdgeL->startVe))
			{
				IsExist = true;break;
			}
		}

		if (!IsExist)
		{
			VertexsList.push_back(Tedge->HalfEdgeL->startVe);//如果这个顶点之前不在清单中，就加入清单。
			outfile << Tedge->HalfEdgeL->startVe->point <<endl;//顺便输出来吧
		}
		Tedge = Tedge->nexte;
	}
	//列出Loops
	for (int i = 0; i < loop_id; i++)
	{
		Loop * ploop = loop_list[i];
		outfile << numEdge(ploop) << " ";
		HalfEdge *tHalfedge = ploop->lEdge;
		while (tHalfedge)
		{
			int ind = 0;
			for each (Vertex *var in VertexsList)
			{			
				if (equal(var, tHalfedge->startVe))
					break;
				ind++;
			}
			outfile << ind << " ";
			tHalfedge = tHalfedge->nxt;
			if (tHalfedge == ploop->lEdge)
				break;
		}
		outfile << endl;
	}
	//列出Faces
	tface = Tsolid->sFace;
	while (tface)
	{
		//先找外环的编号
		for (int i = 0;i < loop_num;i++)
		{
			if (equal(tface->fLoop, loop_list[i]))
			{
				outfile << i << " ";break;
			}
		}
		//再找内环的个数
		Loop *tloop = tface->fLoop;
		int numLp = 0;
		while (tloop != NULL)
		{
			numLp++;
			tloop = tloop->nextl;
		}
		outfile << numLp-1 << " ";
		//再找出内环的编号
		Loop *ploop = tface->fLoop->nextl;
		while (ploop != NULL)
		{
			for (int i = 0;i < loop_num;i++)
			{
				if (equal(ploop, loop_list[i]))
				{
					outfile << i << " ";break;
				}
			}
			ploop = ploop->nextl;
		}
		//face_ind++;
		outfile << 0 << " " << endl;
		tface =tface->nextf;
	}
	outfile.close();
}
int main(int argc, char **argv)
{
	string FenSolid = "inputfen.txt";
	MakeSolid *solidFen = new MakeSolid;
	solidFen->CreateSweepSolid(FenSolid, vec, d);
	Solid *InstanceSolid = solidFen->GetSolid();
	Display::SetDisplaySolid(InstanceSolid);
	OutBrp(InstanceSolid,"fen.brp");
	Display::Run(argc, argv);
	return 0;
}