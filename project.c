#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<stdlib.h>
/*
1) please note that an input txt file is attached with this assignment.
2) you can refer those input of 20 stations as sample input(just need to copy that and paste it while running program) or if you want your own data to be used then just press 1 to add it.
3) please note that sample input provided just adds the database to strucure, further operation can be done as per your wish.
R3-batch:
Enrollment numbers:
1)BT22CSE071
2)BT22CSE074 
*/
int size;//denotes number of stations
struct parameters
{
    float Sky_view_factor,Veg_density_ratio,*Dist_other;
    int Population_density,psurface_fraction,isurface_fraction,id;
    bool status;
    float temp[30],humidity[30];
}*stations;
void initialize(struct parameters*stations)//initially database must not contain garbage values
{
    int ptr;
    for(int i=0;i<size;i++)
    {
        ptr=size;
        stations[i].id=i;
        stations[i].Sky_view_factor=0;
        stations[i].Veg_density_ratio=0;
        stations[i].Population_density=0;
        stations[i].psurface_fraction=0;
        stations[i].isurface_fraction=0;
        stations[i].status=0;
        while(ptr--)
        {
            stations[i].Dist_other[size-1-ptr]=0;
        }
        ptr=30;
        while(ptr--)
        {
           stations[i].temp[29-ptr]=0;
           stations[i].humidity[29-ptr]=0;
        }
    }
}
void add_info(struct parameters*stations)
{
    int ptr;
    printf("Enter Information of all stations :\n");
    for(int i=0;i<size;i++)
    {
        ptr=size;
        printf("Station number %d:\n",i);
        stations[i].id=i;
        printf("Enter Sky view factor (range from 0.1 to 0.9) = ");
        scanf("%f",&stations[i].Sky_view_factor);
        printf("Enter vegetation desity ratio (percentage value) = ");
        scanf("%f",&stations[i].Veg_density_ratio);
        printf("Enter population density (number of people per sq.km) = ");
        scanf("%d",&stations[i].Population_density);
        printf("Enter Pervious surface fraction (range from 0 to 15) = ");
        scanf("%d",&stations[i].psurface_fraction);
        printf("Enter Impervious surface fraction (range from 0 to 15) = ");
        scanf("%d",&stations[i].isurface_fraction);
        printf("Enter Station status (0 stands for rural and 1 stands for urban) = ");
        scanf("%d",&stations[i].status);
        printf("Enter distance of current station from all station including itself(i.e zero kms) (in kms) :\n");
        while(ptr--)
        {
            printf("Enter distance of current station from station no. %d = ",size-1-ptr);
            scanf("%f",&stations[i].Dist_other[size-1-ptr]);
        }
        printf("Enter Air temperature (in degree celsius) and Relative humidity (percentage value) for all 30 days of month :\n");
        ptr=30;
        while(ptr--)
        {
            printf("Day %d : ",30-ptr);
            scanf("%f %f",&stations[i].temp[29-ptr],&stations[i].humidity[29-ptr]);
        }
    }
}
void heat_degree_days(struct parameters*stations)
{
    int heat_days_frequency[size],highest=0;
    float sum=0.0;
    memset(heat_days_frequency,0,4*size);
    for(int i=0;i<30;i++)
    {
        for(int j=0;j<size;j++)
        {
            sum+=stations[j].temp[i];
        }
    }
    sum=sum*(1.0/(30*size));
    for(int i=0;i<size;i++)
    {
        for(int j=0;j<30;j++)
        {
            if(stations[i].temp[j]>sum)++heat_days_frequency[i];
        }
    }
    for(int i=0;i<size;i++)
    {
        if(highest<heat_days_frequency[i])highest=heat_days_frequency[i];
    }
    for(int i=0;i<size;i++)
    {
        if(heat_days_frequency[i]==highest)printf("Station no. %d\n",i);
    }
}
void thermally_comfort(struct parameters*stations)
{
    int stat_num,shortest_dist=-1,stat_number=-1;
    printf("Enter Station number = ");
    scanf("%d",&stat_num);
    if(stations[stat_num].status==1)
    {
        for(int i=0;i<size;i++)
        {
            if(i!=stat_num)
            {
                if(shortest_dist==-1&&stations[i].status==0)
                {
                    shortest_dist=stations[stat_num].Dist_other[i];
                    stat_number=i;
                }
                else if(shortest_dist>stations[stat_num].Dist_other[i]&&stations[i].status==0)
                {
                    shortest_dist=stations[stat_num].Dist_other[i];
                    stat_number=i;
                }
                else if(shortest_dist==stations[stat_num].Dist_other[i]&&stations[i].status==0)
                {
                    if(stations[stat_num].psurface_fraction<stations[i].psurface_fraction)
                    {
                        shortest_dist=stations[stat_num].Dist_other[i];
                        stat_number=i;
                    }
                    else if(stations[stat_num].psurface_fraction==stations[i].psurface_fraction)
                    {
                        if(stations[stat_num].isurface_fraction<stations[i].isurface_fraction)
                        {
                            shortest_dist=stations[stat_num].Dist_other[i];
                            stat_number=i;
                        }
                        else if(stations[stat_num].isurface_fraction==stations[i].isurface_fraction)
                        {
                            if(stations[stat_num].Population_density<stations[i].Population_density)
                            {
                                shortest_dist=stations[stat_num].Dist_other[i];
                                stat_number=i;
                            }
                            else if(stations[stat_num].Population_density==stations[i].Population_density)
                            {
                                if(stations[stat_num].Veg_density_ratio<stations[i].Veg_density_ratio)
                                {
                                    shortest_dist=stations[stat_num].Dist_other[i];
                                    stat_number=i;
                                }
                                else if(stations[stat_num].Veg_density_ratio==stations[i].Veg_density_ratio)
                                {
                                    if(stations[stat_num].Sky_view_factor<stations[i].Sky_view_factor)
                                    {
                                        shortest_dist=stations[stat_num].Dist_other[i];
                                        stat_number=i;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if(stat_number==-1)printf("No rural climate station exist in the city\n");
        else
        {
            int diff_temp=abs(stations[stat_num].temp[0]-stations[stat_number].temp[0]);
            for(int i=1;i<30;i++)
            {
                if(diff_temp>abs(stations[stat_num].temp[i]-stations[stat_number].temp[i]))
                {
                    diff_temp=abs(stations[stat_num].temp[i]-stations[stat_number].temp[i]);
                }
            }
            for(int i=0;i<30;i++)
            {
                if(abs(stations[stat_num].temp[i]-stations[stat_number].temp[i])==diff_temp)printf("Day %d\n",i+1);
            }
        }
    }
    else printf("Entered Climate station is rural\n");
}
void mself(struct parameters*stations1,int l,int m,int h,struct parameters*stations_cpy)
{
	int i=l,j=m+1,k=l;
	while(i<=m&&j<=h)
	{
		if(stations1[i].Veg_density_ratio<stations1[j].Veg_density_ratio)
		{
			stations_cpy[k]=stations1[i];
            ++k,++i;
		}
		else
		{
			stations_cpy[k]=stations1[j];
			++k,++j;
		}
	}
	if(i<=m)
	{
		while(i<=m)
		{
			stations_cpy[k]=stations1[i];
			++k,++i;
		}
	}
	else
	{
		while(j<=h)
		{
			stations_cpy[k]=stations1[j];
			++k,++j;
		}
	}
    for(int t=l;t<=h;t++)
	{
		stations1[t]=stations_cpy[t];
    }
}
void msort(struct parameters*stations1,int l,int h,struct parameters*stations_cpy)
{
	if(l<h)
	{
		msort(stations1,l,(l+h)/2,stations_cpy);
		msort(stations1,(l+h)/2+1,h,stations_cpy);
		mself(stations1,l,(h+l)/2,h,stations_cpy);
	}
}
void decreasing_veg(struct parameters*stations)
{
    float sum=0.0;
    struct parameters stations_cpy[size],stations1[size];
    for(int i=0;i<size;i++)stations1[i]=stations[i];
    msort(stations1,0,size-1,stations_cpy);
    for(int i=0;i<30;i++)
    {
        for(int j=0;j<size;j++)
        {
            sum+=stations[j].temp[i];
        }
    }
    sum=sum*(1.0/(30*size));
    for(int i=size-1;i>=0;i--)
    {
        printf("Station no. %d :\nDates are :",stations1[i].id);
        for(int j=0;j<30;j++)
        {
            if(sum<stations1[i].temp[j])printf("%d ",j+1);
        }
        printf("\n");
    }
}
void top_k(struct parameters*stations)
{
    int Max_temp[size],max,k,tempint;
    struct parameters stations1[size],temp;
    for(int i=0;i<size;i++)
    {
        max=stations[i].temp[0];
        for(int j=1;j<30;j++)
        {
            if(max<stations[i].temp[j])max=stations[i].temp[j];
        }
        Max_temp[i]=max;
        stations1[i]=stations[i];
    }
    printf("Enter k = ");
    scanf("%d",&k);
    if(k>size)printf("k exceeds number of stations\n");
    else
    {
        for(int i=0;i<k;i++)
        {
            for(int j=0;j<size-1-i;j++)
            {
                if(stations1[j].Population_density<stations1[j+1].Population_density)
                {
                    temp=stations1[j];
                    stations1[j]=stations1[j+1];
                    stations1[j+1]=temp;
                    tempint=Max_temp[j];
                    Max_temp[j]=Max_temp[j+1];
                    Max_temp[j+1]=tempint;
                    
                }
                else if(stations1[j].Population_density==stations1[j+1].Population_density)
                {
                    if(Max_temp[j]>Max_temp[j+1])
                    {
                        temp=stations1[j];
                        stations1[j]=stations1[j+1];
                        stations1[j+1]=temp;
                        tempint=Max_temp[j];
                        Max_temp[j]=Max_temp[j+1];
                        Max_temp[j+1]=tempint;
                    }
                }
            }
        }
        for(int i=size-1;i>=size-k;i--)
        {
            printf("Station no.%d\n",stations1[i].id);
        }
    }
}
void heat_island_intensity(struct parameters* stations)
{
    float temp_avg[size];
    memset(temp_avg,0,4*size);
    for(int i=0;i<size;i++)
    {
        for(int j=0;j<30;j++)
        {
            temp_avg[i]+=stations[i].temp[j];        
        }
        temp_avg[i]=temp_avg[i]/30.0;
    }
    int flag,nearby,stat_loc=-1;
    for(int i=0;i<size;i++)
    {
        flag=0;
        if(stations[i].status==1)
        {
            for(int j=0;j<size;j++)
            {
                if(stations[j].status==0&&flag==0)
                {
                    ++flag;
                    nearby=stations[i].Dist_other[j];
                    stat_loc=j;
                }
                else if(stations[j].status==0)
                {
                    if(nearby>stations[i].Dist_other[j])
                    {
                        nearby=stations[i].Dist_other[j];
                        stat_loc=j;
                    }
                    else if(nearby==stations[i].Dist_other[j])
                    {
                        if(stations[stat_loc].psurface_fraction<stations[j].psurface_fraction)
                        {
                            nearby=stations[i].Dist_other[j];
                            stat_loc=j;
                        }
                        else if(stations[stat_loc].psurface_fraction==stations[j].psurface_fraction)
                        {
                            if(stations[stat_loc].isurface_fraction<stations[j].isurface_fraction)
                            {
                                nearby=stations[i].Dist_other[j];
                                stat_loc=j;
                            }
                            else if(stations[stat_loc].isurface_fraction==stations[j].isurface_fraction)
                            {
                                if(stations[stat_loc].Population_density<stations[j].Population_density)
                                {
                                    nearby=stations[i].Dist_other[j];
                                    stat_loc=j;
                                }
                                else if(stations[stat_loc].Population_density==stations[j].Population_density)
                                {
                                    if(stations[stat_loc].Veg_density_ratio<stations[j].Veg_density_ratio)
                                    {
                                        nearby=stations[i].Dist_other[j];
                                        stat_loc=j;
                                    }
                                    else if(stations[stat_loc].Veg_density_ratio==stations[j].Veg_density_ratio)
                                    {
                                          if(stations[stat_loc].Sky_view_factor<stations[j].Sky_view_factor)
                                        {
                                            nearby=stations[i].Dist_other[j];
                                            stat_loc=j; 
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if(stat_loc==-1)printf("No rural Stations are there in city\n");
            else
            {
                printf("Station no. %d:\n",stations[i].id);
                printf("Heat island intensity = %f\n",temp_avg[i]-temp_avg[stat_loc]);
            }
        }
    }
}
void main()
{
    int menu_num;
    initialize(stations);
    printf("Enter number of stations in city = ");
    scanf("%d",&size);
    stations=(struct parameters*)malloc(size*sizeof(struct parameters));
    for(int i=0;i<size;i++)stations[i].Dist_other=(float*)malloc(size*sizeof(float));
    do
    {
        printf("Enter 1 to add Information about %d stations\n",size);
        printf("Enter 2 to list stations with highest heat degree days\n");
        printf("Enter 3 to find the thermally comfortable days for a selected station\n");
        printf("Enter 4 to List dates for all stations with decreasing vegetation density ratio and for which the temperature was more than average city temperature\n");
        printf("Enter 5 to List the (top k, k-input) stations with minimum population density with highest maximum temperature\n");
        printf("Enter 6 to find the heat island intensity for all urban stations\n");
        printf("Enter 7 to exit\n");
        scanf("%d",&menu_num);
        switch (menu_num)
        {
            case 1: add_info(stations);
            break;
            case 2:heat_degree_days(stations);
            break;
            case 3:thermally_comfort(stations);
            break;
            case 4:decreasing_veg(stations);
            break;
            case 5:top_k(stations);
            break;
            case 6:heat_island_intensity(stations);
            break;
        }
    }while(menu_num!=7);
}