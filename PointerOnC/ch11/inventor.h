//包含零件专用信息的结构
typedef struct	{
	int	cost;
	int	supplier;
} Partinfo;

//存储装配件专用信息的结构
typedef	struct	{
	int	n_parts;
	struct	SUBASSYPART {
		char	partno[10];
		short	quan;
	} *part;
} Subassyinfo;

//存货记录结构，他是一个变体记录
typedef	struct	{
	char	partno[10];
	int	quan;
	enum	{ PART, SUBASSY }	type;
	union	{
		Partinfo	*part;
		Subassyinfo	*subassy;
	} info;
} Invrec;

