#include <cstdlib> // for exit
#include "hdt.h"
#include "rooted_tree.h"
#include "hdt_factory.h"
#include "counting_linked_list.h"
#include "counting_linked_list_num_only.h"
#include "soda13_calc.h"

#ifdef quartetsToo

bool HDT::gotoIteratorValueForList(CountingLinkedList *list, unsigned int num)
{
	INC_RUNTIME_COUNTER
	if (list == NULL || list->iteratorHasEnded()) return false;
	list->getIteratorValue(num);
	return !list->iteratorHasEnded();
}

INTTYPE_REST HDT::getIteratorValueForNumList(CountingLinkedListNumOnly *list, unsigned int num)
{
	INC_RUNTIME_COUNTER
	if (list == NULL) return 0;
	return list->getIteratorValue(num);
}

bool HDT::gotoIteratorValueForNumList(CountingLinkedListNumOnly *list, unsigned int num)
{
	INC_RUNTIME_COUNTER
	if (list == NULL || list->iteratorHasEnded()) return false;
	list->getIteratorValue(num);
	return !list->iteratorHasEnded();
}

bool HDT::hasIteratorForNumListEnded(CountingLinkedListNumOnly *list)
{
	INC_RUNTIME_COUNTER
	return list == NULL || list->iteratorHasEnded();
}

void HDT::addToNumList(CountingLinkedList *parent, AddToType list, unsigned int num, INTTYPE_REST value)
{
	INC_RUNTIME_COUNTER
	if (value < 0)
		cout << "WTF?!? Adding '" << value << "' for #" << num << endl;
				
	if (value <= 0) return;

	CountingLinkedListNumOnly *theList;
	bool isReset = false;

	switch(list)
	{
		case i_j: {INITIALIZE_PAREN_AND_SET_LIST(n_i_j, n_i_j_is_reset); break;}
#ifndef calcE
		case paren_i_j: {INITIALIZE_PAREN_AND_SET_LIST(n_paren_i_j, n_paren_i_j_is_reset); break;}
#endif
#ifdef quartetsNoSwap
		case j_arrow_i: {INITIALIZE_PAREN_AND_SET_LIST(n_j_arrow_i, n_j_arrow_i_is_reset); break;}
		case i_arrow_j: {INITIALIZE_PAREN_AND_SET_LIST(n_i_arrow_j, n_i_arrow_j_is_reset); break;}
#ifndef calcE
		case i_paren_i_j: {INITIALIZE_PAREN_AND_SET_LIST(n_i_paren_i_j, n_i_paren_i_j_is_reset); break;}
		case paren_i_paren_i_j: {INITIALIZE_PAREN_AND_SET_LIST(n_paren_i_paren_i_j, n_paren_i_paren_i_j_is_reset); break;}
		case bracket_i_paren_i_j: {INITIALIZE_PAREN_AND_SET_LIST(n_bracket_i_paren_i_j, n_bracket_i_paren_i_j_is_reset); break;}
#endif
#endif
		default: exit(-1);
	}

	if (!isReset)
	{
		// Go to the next one!
		if (theList->iterator->next == NULL) theList->iterator->next = factory->getLLNO();
		theList->iterator->type = CountingLinkedListNumOnly::Regular;
		theList = theList->iterator = theList->iterator->next;
	}

	theList->type = CountingLinkedListNumOnly::End;
	theList->num = num;
	theList->value = value;
}

#endif