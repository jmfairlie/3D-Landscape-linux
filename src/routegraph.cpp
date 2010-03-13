#include "routegraph.h"

RouteGraph::RouteGraph()
{
    this->constructGraph();
}

void RouteGraph::constructGraph()
{
    //construct nodes
    RouteNode *rautatieAsema =
            new RouteNode(VECTOR4D(19.43f, -7.57f, 12.63f, 1.0f), QString("Rautatieasema"));
            //new RouteNode(VECTOR4D(0.0f, 0.0f, 0.0f, 1.0f), QString("Rautatieasema"));
    RouteNode *hameenkatuStart =
            new RouteNode(VECTOR4D(-1.89f, -6.28f, -34.2f, 1.0f), QString("HameenkatuStart"));
            //new RouteNode(VECTOR4D(0.0f, 0.0f, 0.0f, 1.0f), QString("Rautatieasema"));
    RouteNode *hameenSilta =
            new RouteNode(VECTOR4D(-8.27f, -9.47f, -465.9f, 1.0f), QString("HameenSilta"));
            //new RouteNode(VECTOR4D(-20.00f, 0.0f, -450.0f, 1.0f), QString("HameenSilta"));
    RouteNode *keskustori =
            new RouteNode(VECTOR4D(-17.22f, -12.2f, -621.79f, 1.0f), QString("Keskustori"));
            //new RouteNode(VECTOR4D(-20.00f, 0.0f, -620.0f, 1.0f), QString("Keskustori"));

    //construct edges and add them to the nodes
    RouteEdge *rAhS = new RouteEdge(rautatieAsema, hameenkatuStart);
    rautatieAsema->incidentEdges.append(rAhS);
    hameenkatuStart->incidentEdges.append(rAhS);

    RouteEdge *hsHS = new RouteEdge(hameenkatuStart, hameenSilta);
    hameenkatuStart->incidentEdges.append(hsHS);
    hameenSilta->incidentEdges.append(hsHS);

    RouteEdge *hSkT = new RouteEdge(hameenSilta, keskustori);
    hameenSilta->incidentEdges.append(hSkT);
    keskustori->incidentEdges.append(hSkT);

    //add nodes to graph
    this->nodes.append(rautatieAsema);
    this->nodes.append(hameenkatuStart);
    this->nodes.append(hameenSilta);
    this->nodes.append(keskustori);
}

void RouteGraph::renderGraph()
{
    for(int i = 0; i < this->nodes.size(); i++)
    {
        for(int j = 0; j < this->nodes[i]->incidentEdges.size(); j++)
        {
            glColor3f(1.0f, 0.0f, 0.0f);
            glLineWidth(2.0f);

            cout << "LINE!!!" << endl;

            glBegin(GL_LINES);
                glVertex3f(this->nodes[i]->incidentEdges[j]->firstEP->coords.x,
                           this->nodes[i]->incidentEdges[j]->firstEP->coords.y,
                           this->nodes[i]->incidentEdges[j]->firstEP->coords.z);
                glVertex3f(this->nodes[i]->incidentEdges[j]->secondEP->coords.x,
                           this->nodes[i]->incidentEdges[j]->secondEP->coords.y,
                           this->nodes[i]->incidentEdges[j]->secondEP->coords.z);
            glEnd();
        }
    }
}

RouteGraph::~RouteGraph()
{
    for(int i = 0; i < this->nodes.size(); i++)
    {
        delete this->nodes[i];        
    }    
    this->nodes.clear();
}
