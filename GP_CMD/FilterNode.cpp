#include "stdafx.h"
#include "FilterNode.h"

FilterNode::FilterNode(const FilterNode & orig):
	functionMessage(orig.functionMessage),
	family(orig.family)
{
}

FilterNode::FilterNode(const pair<bool, size_t> message)
{
}


FilterNode::~FilterNode()
{
}

FilterNode & FilterNode::operator=(const FilterNode & orig)
{
	// TODO: 在此处插入 return 语句
	functionMessage = orig.functionMessage;
	family = orig.family;
	return *this;
}

void FilterNode::getInput()
{
	if ((!leftChild) && (!rightChild));
	else if (leftChild && rightChild)
		input = { leftChild->output, rightChild->output };
	else
		leftChild ? input = { leftChild->output } : input = { rightChild->output };
}

void FilterNode::getOutput()
{
	getInput();
	ImageFunction function = functionMessage.first ?
		twoInputFunction[functionMessage.second] : oneInputFunction[functionMessage.second];
	function(input, output);
}
