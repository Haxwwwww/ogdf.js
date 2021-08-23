#include <ogdf/planarity/PlanarizationGridLayout.h>
#include "../main.h"

EM_PORT_API(float *) PG(int node_num, int link_num, int* source, int* target, double pageRatio, double separation, int crossMinType, int globalInternalLibraryLogLevel, int globalLogLevel, int globalMinimumLogLevel, bool globalStatisticMode, int localLogLevel, int localLogMode, unsigned int maxThreads, int permutations, bool timeout, double timeLimit, int inserterType, bool keepEmbedding, double percentMostCrossed, int removeReinsert, double percentMostCrossedFix, double percentMostCrossedVar, int removeReinsertFix, int removeReinsertVar, bool statistics, int subgraphType, int runs, double randomness, int packerType, int planarLayouterType, int augmenterType, int crossingsBeautifierType, int embedderType, bool useExtendedDepthDefinition, int shellingOrderType, double baseRatio) {
	node* nodes;
	Graph G;
	GraphAttributes GA(G, GraphAttributes::nodeGraphics | GraphAttributes::edgeGraphics);
	

	nodes = new node[node_num];
	for (int i = 0; i < node_num; i++){
		nodes[i] = G.newNode();
	}

	edge e;

	for (int i = 0; i < link_num; i++) {
		e = G.newEdge(nodes[source[i]], nodes[target[i]]);
		GA.bends(e);
	}

	//LayoutModule
	PlanarizationGridLayout *model = new PlanarizationGridLayout();
	
	model->pageRatio(pageRatio);
    model->separation(separation);

	CrossingMinimizationModule* crossMin = getCrossingMinimization(crossMinType, globalStatisticMode, globalLogLevel, globalMinimumLogLevel, globalStatisticMode, localLogLevel, localLogMode, maxThreads, permutations, timeout, timeLimit, inserterType, keepEmbedding, percentMostCrossed, removeReinsert, percentMostCrossedFix, percentMostCrossedVar, removeReinsertFix, removeReinsertVar, statistics, subgraphType, runs, randomness);
	model->setCrossMin(crossMin);

	CCLayoutPackModule* packer = getCCLayoutPack(packerType);
	model->setPacker(packer);

	GridLayoutPlanRepModule* planarLayouter = getGridLayoutPlanRep(planarLayouterType, separation, augmenterType, crossingsBeautifierType, embedderType, timeLimit, useExtendedDepthDefinition, shellingOrderType, baseRatio);
	model->setPlanarLayouter(planarLayouter);

	model->call(GA);
	
	float* re = (float*)malloc(node_num * 2 * 4);
    for(int i = 0; i < node_num; ++i) {
        re[i * 2] = GA.x(nodes[i]);
        re[i * 2 + 1] = GA.y(nodes[i]);
    }

    return re;
}