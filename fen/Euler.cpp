
#include "lib.h"
#include "HalfEdge.h"

Solid * Euler::mvfs(Point point, Vertex *& newVertex)
{
	Solid *newSolid = new Solid;
	Face *newFace = new Face;
	Loop *newLoop = new Loop;//外环

	newVertex = new Vertex;
	newVertex->point.SetCoord(point);

	newSolid->sFace = newFace;
	newFace->fSolid = newSolid;
	newFace->fLoop = newLoop;
	newLoop->lFace = newFace;
	newLoop->lEdge = NULL;
	return newSolid;
}

HalfEdge * Euler::mev( Vertex *v1, Point p2, Loop *lp )
{
	Solid *solid = lp->lFace->fSolid;
	HalfEdge *he1 = new HalfEdge;
	HalfEdge *he2 = new HalfEdge;
	HalfEdge *he = new HalfEdge;
	Edge *edge = new Edge;

	Vertex *v2 = new Vertex;
	v2->point.SetCoord(p2);

	he1->edge = he2->edge = edge;
	edge->HalfEdgeL = he1; 
	edge->HalfEdgeR = he2;
	
	he1->hLoop = he2->hLoop = lp;
	he1->startVe = v1;
	he1->endVe = v2;
	he2->startVe = v2;
	he2->endVe = v1;

	he1->adjacentHe = he2;
	he2->adjacentHe = he1;

	if (lp->lEdge == NULL)
	{
		he1->nxt = he2;
		he2->prv = he1;
		he2->nxt = he1;
		he1->prv = he2;
		lp->lEdge = he1;
	}
	else
	{
		for (he = lp->lEdge; he->nxt->startVe != v1; he = he->nxt);

		he1->nxt = he2;
		he1->prv = he;
		he2->nxt = he->nxt;
		he2->prv = he1;
		he->nxt->prv = he2;
		he->nxt = he1;
	}

	Edge *curEdge = solid->edge;
	while (curEdge != NULL && curEdge->nexte != NULL)
	{
		curEdge = curEdge->nexte;
	}
	if (curEdge == NULL)	solid->edge = edge;
	else
	{
		curEdge->nexte = edge;
		edge->preve = curEdge;
	}

	return he1;
}

Loop * Euler::mef( Vertex *v1, Vertex *v2, Loop *lp )
{
	Solid *solid = lp->lFace->fSolid;
	Face *face = new Face;
	Loop *loop = new Loop;//内环
	HalfEdge *he1 = new HalfEdge;
	HalfEdge *he2 = new HalfEdge;
	Edge *edge = new Edge;

	he1->startVe = v1;
	he1->endVe = v2;
	he2->startVe = v2;
	he2->endVe = v1;
	he1->adjacentHe = he2;
	he2->adjacentHe = he1;

	edge->HalfEdgeL = he1;
	edge->HalfEdgeR = he2;
	he1->edge = he2->edge = edge;

	HalfEdge *tmphe1, *tmphe2, *tmphe;
	for (tmphe = lp->lEdge; tmphe->startVe != v1; tmphe = tmphe->nxt);
	tmphe1 = tmphe;

	for (; tmphe->startVe != v2; tmphe = tmphe->nxt);
	tmphe2 = tmphe;
	tmphe = tmphe->nxt;
	while (tmphe->startVe != v2)
	{
		tmphe = tmphe->nxt;
	}
	bool HaveRoll = false;
	if(tmphe != tmphe2)
	{
		HaveRoll = true;
		tmphe2 = tmphe;
	}

	he1->nxt = tmphe2;
	he1->prv = tmphe1->prv;
	he2->nxt = tmphe1;
	he2->prv = tmphe2->prv;

	tmphe1->prv->nxt = he1;
	tmphe1->prv = he2;

	tmphe2->prv->nxt = he2;
	tmphe2->prv = he1;

	loop->lEdge = he1;	//Inner loop
	lp->lEdge = he2;

	face->fLoop = loop;
	face->fSolid = solid;
	loop->lFace = face;
	//Add Face to Solid
	Face *tmpFace;
	for (tmpFace = solid->sFace; tmpFace->nextf != NULL; tmpFace = tmpFace->nextf);
	tmpFace->nextf = face;
	face->prevf = tmpFace;
	face->fSolid = solid;

	Edge *curEdge = solid->edge;
	while (curEdge != NULL && curEdge->nexte != NULL)
	{
		curEdge = curEdge->nexte;
	}
	if (curEdge == NULL)	solid->edge = edge;
	else
	{
		curEdge->nexte = edge;
		edge->preve = curEdge;
	}

	return loop;

}

Loop * Euler::kemr( Vertex *v1, Vertex *v2, Loop *lp )
{
	Face *face = lp->lFace;
	Solid *solid = face->fSolid;
	Loop *loop = new Loop;
	HalfEdge *he;
	for (he = lp->lEdge; (he->startVe != v2) || (he->endVe != v1); he = he->nxt);
	
	Edge *edge;
	edge = he->edge;
	
	he->nxt->prv = he->adjacentHe->prv;
	he->adjacentHe->prv->nxt = he->nxt;
	
	he->prv->nxt = he->adjacentHe->nxt;
	he->adjacentHe->nxt->prv = he->prv;

	lp->lEdge = he->nxt->prv;

	loop->lEdge = he->prv;
	loop->lFace = face;

	if (face->fLoop == NULL)
		face->fLoop = loop;
	else
	{
		Loop *tmpLoop;
		for (tmpLoop = face->fLoop; tmpLoop->nextl != NULL; tmpLoop = tmpLoop->nextl);
		tmpLoop->nextl = loop;
		loop->prevl = tmpLoop;
	}

	Edge *tmpEdge = solid->edge;
	while ( tmpEdge != edge)
	{
		tmpEdge = tmpEdge->nexte;
	}
	//delete edge
	if (tmpEdge->nexte == NULL)
	{
		tmpEdge->preve->nexte = NULL;
	}
	else if(tmpEdge->preve == NULL)
	{
		solid->edge = tmpEdge->nexte;
		tmpEdge->nexte->preve = NULL;
	}
	else
	{
		tmpEdge->preve->nexte = tmpEdge->nexte;
		tmpEdge->nexte->preve = tmpEdge->preve;
	}
	delete tmpEdge;
	return loop;
}

void Euler::kfmrh( Loop *outlp, Loop *lp )
{

	Face *face1 = outlp->lFace;
	Face *face2 = lp->lFace; // 里面环对应的face删掉，环变为内环。
	if (face1->fLoop == NULL)
		face1->fLoop = lp;
	else
	{
		Loop *tmpLoop;
		for (tmpLoop = face1->fLoop; tmpLoop->nextl != NULL; tmpLoop = tmpLoop->nextl);
		tmpLoop->nextl = lp;
		lp->prevl = tmpLoop;
	}
	lp->lFace = face1;

	Solid *solid = face1->fSolid;
	Face *tmpFace = solid->sFace;
	while ( tmpFace != face2)
	{
		tmpFace = tmpFace->nextf;
	}
	if (tmpFace->nextf == NULL)
	{
		tmpFace->prevf->nextf = NULL;
	}
	else if(tmpFace->prevf == NULL)
	{
		solid->sFace = tmpFace->nextf;
		tmpFace->nextf->prevf = NULL;
	}
	else
	{
		tmpFace->prevf->nextf = tmpFace->nextf;
		tmpFace->nextf->prevf = tmpFace->prevf;
	}
	delete tmpFace;
}

void Point::SetCoord( double x, double y, double z )
{
	coord[0] = x;
	coord[1] = y;
	coord[2] = z;
}

void Point::SetCoord( Point point )
{
	coord[0] = point.coord[0];
	coord[1] = point.coord[1];
	coord[2] = point.coord[2];
}
