template <class entity_type>
class State
{
public:
	virtual void Execute(entity_type*)=0;
	virtual ~State(){}
};
