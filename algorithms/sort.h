#ifndef  _ALGORITHMS_SORT_H
#define  _ALGORITHMS_SORT_H

namespace  Common{

template <typename T>
typedef bool (*CompareFunc)(T,T);

//堆排序
int Parent(int& pos)
{ 
	return pos >> 1 ;
}

int ChildLeft(int& pos)
{ 
	return pos << 1;
}

int ChildRight(int& pos)
{ 
	return pos << 1 + 1;
}

template <typename T>
bool max_heapify(T *datas, int datalen, int pos, CompareFunc cmd=NULL)
{ 
	int left = 
	return true;
}

template <typename T> 
int heap_sort(T *datas, int datalen, CompareFunc cmp = NULL)
{ 
	for(int i = 1; i <= datalen; i++)
	{ 
	
	}

}


}
#endif
