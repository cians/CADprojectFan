
#include "lib.h"
#include "HalfEdge.h"

MakeSolid::MakeSolid()
{
	_Euler = new Euler();
}

Solid * MakeSolid::CreateSweepSolid(string iPath,double vec[3], double d)
{
	Solid *solid;
	Loop *loop, *loopOnFace, *innerLoop;
	Face *face;
	loop = new Loop;
	solid = new Solid;
	Point point;
	Vertex *v1, *v2;

	int numOfIns, numOfPoint, type, index1, index2, indexOfLoop;
	fstream fs;
	fs.open(iPath.c_str(),ios::in);
	fs >> numOfPoint >> numOfIns;
	vector< Vertex* > VertexList(numOfPoint);
	vector< Point > PointList(numOfPoint);
	vector< Loop* > LoopList;

	for (int i = 0; i < numOfPoint; i++)
	{
		fs >> point;
		PointList[i] = point;
	}
	//create Solid
	for (int i = 0; i < numOfIns; i++)
	{
		fs >> type;
		switch (type)
		{
		case 0:
			//mvfs
			fs >> index1;
			Vertex *tmpVertex;
			solid = _Euler->mvfs( PointList[index1], tmpVertex);
			loop = solid->sFace->fLoop;
			LoopList.push_back(loop);
			VertexList[index1] = tmpVertex;
			break;
		case 1:
			//mev
			HalfEdge *he;
			fs >> index1 >> index2;
			v1 = VertexList[index1];
			point = PointList[index2];
			he = _Euler->mev(VertexList[index1], PointList[index2], loop);
			VertexList[index2] = he->endVe;
			break;
		case 2:
			//mef
			fs >> index1 >> index2;
			v1 = VertexList[index1];
			v2 = VertexList[index2];
			loopOnFace = _Euler->mef(v1, v2, loop);
			LoopList.push_back(loopOnFace);
			break;
		case 3:
			//kemr
			fs >> index1 >> index2;
			v1 = VertexList[index1];
			v2 = VertexList[index2];
			innerLoop = _Euler->kemr(v1, v2, loop);
			LoopList.push_back(innerLoop);
			break;
		case 4:
			//kfmrh
			fs >> index1 >> index2;
			_Euler->kfmrh(LoopList[index1], LoopList[index2]);
			break;
		case 5:
			//select loop
			fs >> indexOfLoop;
			loop = LoopList[indexOfLoop];
			break;
		case 6:
			//Sweep
			face = solid->sFace;
			Sweep(face, vec, d);
			break;
		default:;
		}
	}
	fs.close();

	Face *tempf = solid->sFace;
	Loop *templ;
	HalfEdge *start,*temphl;
	while (tempf != NULL)
	{
		templ = tempf->fLoop;
		while (templ!=NULL)
		{
			temphl = templ->lEdge->nxt;
			start = templ->lEdge;
			while (temphl!= start)
			{
				temphl = temphl->nxt;
			}
			templ = templ->nextl;
		}
		tempf = tempf->nextf;
	}

	Solid *pCurSolid = _Euler->solid;
	while (pCurSolid != NULL && pCurSolid->nexts != NULL)
	{
		pCurSolid = pCurSolid->nexts;
	}
	if (pCurSolid == NULL)	_Euler->solid = solid;
	else
	{
		solid->prevs = pCurSolid;
		pCurSolid->nexts = solid;
	}
	return _Euler->solid;
}

void MakeSolid::Sweep( Face *face, double vec[3], double d )
{
	Solid *solid = face->fSolid;
	Loop *loop;
	HalfEdge *he;
	Vertex *firstv;
	Vertex *upvertex;
	Vertex *prevupvertex;
	Vertex *nextv;
	Point uppoint;
	HalfEdge *uphe;
	HalfEdge *firstuphe;

	for (loop = face->fLoop; loop != NULL; loop = loop->nextl)
	{
		he = loop->lEdge;
		firstv = he->startVe;
		uppoint.SetCoord(firstv->point.coord[0] + d*vec[0],
			firstv->point.coord[1] + d*vec[1],
			firstv->point.coord[2] + d*vec[2]);
		firstuphe = _Euler->mev(firstv, uppoint, loop);
		prevupvertex = firstuphe->endVe;
		he = he->nxt;
		nextv = he->startVe;
		while (nextv != firstv)
		{
			uppoint.SetCoord(nextv->point.coord[0] + d*vec[0],
				nextv->point.coord[1] + d*vec[1],
				nextv->point.coord[2] + d*vec[2]);
			uphe = _Euler->mev(nextv, uppoint, loop);
			upvertex = uphe->endVe;
			_Euler->mef(upvertex, prevupvertex, loop);
			prevupvertex = upvertex;
			he = he->nxt;
			nextv = he->startVe;
		}
		_Euler->mef(firstuphe->endVe, prevupvertex, loop);
	}
}

Solid * MakeSolid::GetSolid()
{
	return _Euler->solid;
}


