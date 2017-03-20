/***********************************************************************/
/*	���Ի���ģ���㷨����������ʶ�𷵻ص�speedֵ����ֵֻ����0-6֮�������ֵ
	����	int speed_step(int speed, int height, int step_table[]) ����
	
	����˵����
	������int speedΪ����ʶ�𷵻صĻ����ٶ�ֵ
		  int heightΪ����Ӧ��ʱ������峤�ȣ�����96������
		  int step_table[]������Ҫ�û����ٿռ��С��92��int����
	����ֵ��int
			���ص�Ϊ�㷨����step_table[]������ĳ��ȡ�Ҳ�����û���Ҫ�õ�������ĳ���
			���磺92��80����			 
*/
/***********************************************************************/

static int table[7][3] = 
{
	{0, 0, 0},
	{1, 1, 1},
	{1, 2, 1},
	{4, 3, 1},
	{5, 4, 2},
	{6, 5, 2},
	{8, 6, 2}		
};

int step_table[128];

int speed_step(int speed, int height, int step_table[])
{
	int n;
	int i, j;
	int coef[3] = {5, 8, 16};
	
	if(speed == 0)
	{		
		return 0;
	}

	n = 0;
	for(i=0; i<3; i++)
	{		
		
		for(j=0; j<table[speed][i]*coef[i]; j++)
		{
			step_table[n+j] = height/coef[i];
		}
		n += table[speed][i]*coef[i];
	}		
	return n;
}

int speed_step2(int speed, int height, int step_table[])
{
	int n;
	int i,j;
	
	if(speed == 0)
	{
		return 0;
	}
	
	n =0;
	for(i = 0; i < 2 * speed; i++)
	{
		step_table[i] = height;
		n++; 
	}
	
	for(i = 0 ; i < speed * height / 10 ; i++)
	{
		step_table[n + i] = 10;		
	}	
	n +=  speed * height / 10;

	for(j = 0; j < speed * height - 10 * i; j++)
	{
		step_table[n + j] = 1;		
	}
	n += speed * height - 10 * i;
	return n;
}


