//gcc 140101072.c -o 140101072 -lm
// ./140101072 1 2 0 test.txt > output.txt 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

int global_depth ;
int delete_option;
int bucket_size ;
int overflow;
struct node
{
	int *p;
	int ldepth;
	int number;
	int left;
	struct node *next;
};

struct node2
{
	int *p;
	int ldepth;
	int number;
	int left;
	int over_left;
	int *over;
	struct node2 *next;
};
int hash(int data)
{
	int hash_fun =  (int) pow((double) 2,global_depth);
	return data % hash_fun;
}
int d2b(int dec)
{
    if (dec == 0)
    {
        return 0;
    }
    else
    {
        return (dec % 2 + 10 * d2b(dec / 2));
    }
}
void search(int data,struct node **head)
{
	struct node *pass,*new;
	int i=0;
	int flag = 0;
	pass = *head;
	new = *head;
	int hased_value = hash(data);
	while(hased_value != pass->number)
	{
		pass = pass->next;
	}
	for(i=0;i<bucket_size;i++)
	{
		if(data == pass->p[i])
		{
			printf("%d found in bucket %d \n",data,pass->number );
			flag = 1;
			break;
		}
	}
	if(flag ==0)
		printf("%d not found\n",data );

}
int search2(int data,struct node2 **head)
{
	struct node2 *pass,*new;
	int i=0;
	pass = *head;
	new = *head;
	int flag = 0;
	int hased_value = hash(data);
	while(hased_value != pass->number)
	{
		pass = pass->next;
	}
	for(i=0;i<bucket_size;i++)
	{
		if(data == pass->p[i])
		{
			printf("%d found in bucket %d \n",data,pass->number );
			break;
		}
		if(data == pass->over[i])
		{
			printf("%d found in bucket %d  in overflow\n",data,pass->number );
			break;
		}
	}
	if(flag ==0)
		printf("%d not found\n",data );

}
void delete_1 (int data , struct node **head)
{
	struct node *pass,*new;
	int i=0;
	pass = *head;
	new = *head;
	int hased_value = hash(data);
	while(hased_value != pass->number)
	{
		pass = pass->next;
	}
	while(i<bucket_size)
	{
		if(data == pass->p[i])
		{
			pass->p[i] = pass->p[bucket_size-(pass->left)-1];
			(pass->left)++;
			break;
		}
		i++;

	}
	while(new != NULL)
	{
		if(new->p == pass->p)
			new->left = pass->left;
		new = new->next;
	}
}


void delete_12 (int data , struct node2 **head)
{
	struct node2 *pass,*new;
	int i=0;
	pass = *head;
	new = *head;
	int hased_value = hash(data);
	int flag = 0;
	while(hased_value != pass->number)
	{
		pass = pass->next;
	}

	while(i<bucket_size)
	{
		if(data == pass->over[i])
		{
			pass->over[i] = pass->over[bucket_size-(pass->over_left)-1];
			(pass->over_left)++;
			flag = 1;
			break;
		}
		i++;
	}
	i = 0;
	if(flag == 0)
	{
		while(i<bucket_size)
		{
			if (data == pass->p[i])
			{
				if(pass->over_left < bucket_size)
				{
					pass->p[i] = pass->over[bucket_size-(pass->over_left)-1];
					(pass->over_left)++;
				}
				else if(pass->over_left == bucket_size)
				{
					//printf("%d \n", pass->p[i] );
					pass->p[i] = pass->p[bucket_size-(pass->left)-1];
					(pass->left)++;
			
				}
				flag = 1;
				break;
			}
			i++;
		}
		
	}

	while(new != NULL)
	{
		if(new->p == pass->p)
		{
			new->left = pass->left;
			new->over_left = pass->over_left;
		}
		new = new->next;
	}
}

void double_dir(struct node **head)
{
	int count = 1,i,j;
	struct node *pass,*new;
	pass = *head;
	new = *head;
	while(pass->next != NULL)
	{
		pass = pass->next;
		count++;
	}
	j = count;

	//  for least significant bit 
	for(i=0;i<count;i++)
	{
		pass->next = (struct node*) malloc(sizeof(struct node));		//copying data from upper dir
		pass = pass->next;
		pass->p = new->p;
		pass->ldepth = new->ldepth;
		pass->left = new->left;
		pass->number = j;
		j++;
		pass->next = NULL;
		new = new->next;
	}
	// global_depth++;

}

void double_dir2(struct node2 **head)
{
	int count = 1,i,j;
	struct node2 *pass,*new;
	pass = *head;
	new = *head;
	while(pass->next != NULL)
	{
		pass = pass->next;
		count++;
	}
	j = count;

	//  for least significant bit 
	for(i=0;i<count;i++)
	{
		pass->next = (struct node2*) malloc(sizeof(struct node2));		//copying data from upper dir
		pass = pass->next;
		pass->p = new->p;
		pass->ldepth = new->ldepth;
		pass->left = new->left;
		pass->number = j;
		j++;
		pass->next = NULL;
		pass->over_left = new->over_left;
		pass->over = new->over;
		new = new->next;
	}
	// global_depth++;

}


void add_bucket(int data,struct node **head,struct node **curr)
{
	struct node *add_buc,*pass;
	pass = *curr;
	add_buc = *head;
	int i,steps,hased_value;
	int trav,j=0;
	//hased_value = hash(data);
	steps = (int) pow((double) 2,global_depth-1)+pass->number;
	//printf("hased_value = %d\n",hased_value );
	for(i=0;i<steps;i++)
	{
		add_buc = add_buc->next;
	}
	// while(add_buc->number != hased_value)
	// 	add_buc = add_buc->next;

	add_buc->p =  (int*) malloc(bucket_size * sizeof(int)); // create new bucket
	add_buc->left = bucket_size;
	add_buc->ldepth++;
	pass->ldepth++;

	// copyng old data
	for(i=0;i<bucket_size;i++)
	{
		hased_value = hash(pass->p[i]);
		if(hased_value == pass->number)
		{
			pass->p[j] = pass->p[i];
			j++;
		}
		else
		{
			add_buc->p[bucket_size - add_buc->left] = pass->p[i];
			add_buc->left = (add_buc->left) -1;

		}
	}
	pass->left = bucket_size -j;

	// enter new data 
	hased_value = hash(data);
	if(hased_value == pass->number)
	{
		pass->p[bucket_size-pass->left] = data;
		pass->left = (pass->left)-1;
	}
	else
	{
		add_buc->p[bucket_size - add_buc->left] = data;
		add_buc->left = (add_buc->left) -1;
	}

}

void add_bucket2(int data,struct node2 **head,struct node2 **curr)
{
	struct node2 *add_buc,*pass;
	pass = *curr;
	add_buc = *head;
	int i,steps,hased_value;
	int trav,j=0,k=0,l=0,m=0;
	int copy_data;
	//hased_value = hash(data);
	steps = (int) pow((double) 2,global_depth-1)+pass->number;
	//printf("hased_value = %d\n",hased_value );
	for(i=0;i<steps;i++)
	{
		add_buc = add_buc->next;
	}
	// while(add_buc->number != hased_value)
	// 	add_buc = add_buc->next;

	add_buc->p =  (int*) malloc(bucket_size * sizeof(int)); // create new bucket
	add_buc->left = bucket_size;
	add_buc->over_left = bucket_size;
	add_buc->over = (int*) malloc(bucket_size * sizeof(int));
	add_buc->ldepth++;
	pass->ldepth++;

	// copyng old data
	int count = 2*bucket_size - pass->left - pass->over_left;

	//printf("%d  %d\n",pass->left , add_buc->left );
	for(i=0;i<count;i++)
	{
		if(i<bucket_size)
			copy_data = pass->p[i];
		else
			copy_data = pass->over[i- bucket_size ];

		hased_value = hash(copy_data);

		if(hased_value == pass->number)
		{
			if(j<bucket_size)
			{
				pass->p[j] = copy_data;
				j++;
			}
			else
			{
				pass->over[k] = copy_data;
				k++;
			}
		}
		else
		{
			if(l<bucket_size)
			{
				add_buc->p[l] = copy_data;
				l++;
			}
			else
			{
				add_buc->over[m] = copy_data;
				m++;
			}
			/*
			if(add_buc->left > 0)
			{
				add_buc->p[bucket_size - add_buc->left] = copy_data;
				add_buc->left = (add_buc->left) -1;
			}
			else
			{
				add_buc->over[bucket_size - add_buc->over_left] = copy_data;
				add_buc->over_left = (add_buc->over_left) -1;
			}*/
			

		}
	}

	pass->left = bucket_size -j;
	pass->over_left = bucket_size - k;
	add_buc->left = bucket_size - l;
	add_buc ->over_left = bucket_size -m;
	//printf("%d  %d\n",pass->left , add_buc->left );
	//index_status2(head);
	// enter new data 
	hased_value = hash(data);
	if(hased_value == pass->number)
	{
		if(pass->left > 0)
		{
			pass->p[bucket_size-pass->left] = data;
			pass->left = (pass->left)-1;
		}
		else
		{
			pass->over[bucket_size - pass->over_left] = data;
			pass->over_left = (pass->over_left) -1;
		}
	}
	else
	{
		if(add_buc->left > 0)
		{
			add_buc->p[bucket_size - add_buc->left] = data;
			add_buc->left = (add_buc->left) -1;
		}
		else
		{
			add_buc->over[bucket_size - add_buc->over_left] = data;
			add_buc->over_left = (add_buc->over_left) -1;
		}
		
	}

}
void table2(struct node2 **head)
{
	struct node2 *pass,*new; 
	pass = *head;
	new = *head;
	int count;
	while(pass != NULL)
	{
		count = d2b(pass->number) % (int)pow(10,pass->ldepth) ; 
		printf("%d \t %d\n", d2b(pass->number),count);
		pass = pass->next;
	}
}
void table(struct node **head)
{
	struct node *pass,*new; 
	pass = *head;
	new = *head;
	int count;
	while(pass != NULL)
	{
		count = d2b(pass->number) % (int)pow(10,pass->ldepth) ; 
		printf("%d \t %d\n", d2b(pass->number),count);
		pass = pass->next;
	}
}
void index_status(struct node **head)
{
	table(head);
	struct node *pass;
	pass = *head;
	int i ;
	printf("global_depth  = %d\n",global_depth);
	while(pass != NULL)
	{
		if(pass->number<pow(2,pass->ldepth))
		{
			printf("%d\t ----> \t",d2b(pass->number) );
			//printf("%d\t %d\t %d\t\t",pass->number,pass->ldepth,pass->left );
			for(i=0;i<bucket_size - pass->left ; i++)
			{
				printf("%d\t", pass->p[i]);
			}
			if (pass->left==bucket_size)
			{
				printf("empty bucket\t");
				/* code */
			}
			printf("\n");
		}
		pass = pass->next;
	}
}
void index_status2(struct node2 **head)
{
	table2(head);
	struct node2 *pass;
	pass = *head;
	int i ;
	printf("global_depth  = %d\n",global_depth);
	while(pass != NULL)
	{
		if(pass->number<pow(2,pass->ldepth)){
			printf("%d\t ---> \t",d2b(pass->number) );

			for(i=0;i<bucket_size - pass->left ; i++)
			{
				printf("%d\t", pass->p[i]);
			}
			if (pass->left==bucket_size)
			{
				printf("empty bucket\t");
				/* code */
			}
			for(i=0;i<bucket_size - pass->over_left;i++)
				printf("%d\t",pass->over[i] );
			if (pass->over_left==bucket_size)
			{
				printf("empty overflow");
				/* code */
			}


			printf("\n");
	}
		pass = pass->next;
	}
}
void insert_data2(int data,struct node2 **head)
{
	int hased_value = hash(data);
	//printf(" hased_value : %d\n",hased_value );
	struct node2 *pass,*update_left;
	pass = *head;
	update_left = *head;
			// for distrubuting bucket

	while(hased_value != pass->number)
	{
		pass = pass->next;
	}
	//printf("number %d\n",pass->number );
	if(pass->left > 0)
	{
		pass->p[bucket_size - pass->left] = data;
		while(update_left != NULL)
		{
			if(update_left->p == pass->p)
				update_left->left = (update_left->left) -1;

			update_left = update_left->next;
		}


		//printf("in 1st %d\n",data );
	}
	else if(pass->left == 0)
	{

			if(pass->ldepth == global_depth)
			{
				if(pass->over_left > 0)
				{
					pass->over[bucket_size - pass->over_left] = data;
					pass->over_left = pass->over_left -1;
				}
				else
				{	
					double_dir2(head);
					global_depth++;
					//index_status(head);
					add_bucket2(data,head,&pass);
				}
				//printf("in 2st %d\n",data );
			}
			
			else if (pass->ldepth < global_depth)
			{
				if(pass->number >= (int) pow((double) 2,global_depth-1))
				{
					hased_value = data % (int) pow((double) 2,pass->ldepth);
					pass = *head;
					while(hased_value != pass->number)
					{
						pass = pass->next;
					}
				}
				//printf("in 3.1st %d\n",hased_value );
				add_bucket2(data,head,&pass);
				//printf("in 3st %d\n",data );
			}
		
		
	}
}

void insert_data(int data,struct node **head)
{
	int hased_value = hash(data);
	//printf(" hased_value : %d\n",hased_value );
	struct node *pass,*update_left;
	pass = *head;
	update_left = *head;
			// for distrubuting bucket

	while(hased_value != pass->number)
	{
		pass = pass->next;
	}
	//printf("number %d\n",pass->number );
	if(pass->left > 0)
	{
		pass->p[bucket_size - pass->left] = data;
		while(update_left != NULL)
		{
			if(update_left->p == pass->p)
				update_left->left = (update_left->left) -1;

			update_left = update_left->next;
		}


		//printf("in 1st %d\n",data );
	}
	else if(pass->left == 0)
	{

		if(pass->ldepth == global_depth)
		{
			double_dir(head);
			global_depth++;
			//index_status(head);
			add_bucket(data,head,&pass);
			//printf("in 2st %d\n",data );
			
		}
		else if (pass->ldepth < global_depth)
		{
			if(pass->number >= (int) pow((double) 2,global_depth-1))
			{
				hased_value = data % (int) pow((double) 2,pass->ldepth);
				pass = *head;
				while(hased_value != pass->number)
				{
					pass = pass->next;
				}
			}
			//printf("in 3.1st %d\n",hased_value );
			add_bucket(data,head,&pass);
			//printf("in 3st %d\n",data );
		}
	}
}


int main(int argc , char * argv[])
{
	char *p[100];
	int data,k=0,i;
	char *line = NULL;
	size_t len =0;
	ssize_t read;
	if(argc != 5)
	{
		printf("not full arguments\n");
		return 0;
	}
	delete_option = 0;
	global_depth = atoi(argv[1]);
	bucket_size = atoi(argv[2]);
	overflow = atoi(argv[3]);

	// for global_depth = 0	
	
	
	if(overflow==0)
	{
		struct node *pass;
		struct node *head = (struct node*) malloc(sizeof(struct node));
		head->ldepth = global_depth;
		head->number = 0;
		head->p = (int*) malloc(bucket_size * sizeof(int));
		head->left = bucket_size;
		head->next = NULL;
		if(global_depth > 0)
		{
			for(i=0;i<global_depth;i++)
				double_dir(&head);
		}

		pass = head->next;
		while(pass != NULL)
		{
			pass->ldepth = global_depth;
			pass->p = (int*) malloc(bucket_size * sizeof(int));
			pass = pass->next;
		}




		FILE *fp = fopen(argv[4],"r");
		if(fp ==0)
			exit(EXIT_FAILURE);

		while ((read = getline(&line, &len, fp)) != -1) 
		{


			p[0] = strtok(line, " ");
			i = 0;

			while(p[i] != NULL)
			{
				i++;
				p[i] = strtok(NULL," ");
			}

			//printf("%s %s\n",p[0],p[1]);
			if(!strcmp(p[0],"insert"))
			{
				//printf("1\n");
				insert_data(atoi(p[1]),&head);
			}
			else if (!strcmp(p[0],"delete"))
			{
				delete_1(atoi(p[1]),&head);
			}
			else if(!strcmp(p[0],"search"))
			{
				search(atoi(p[1]),&head);
			}

			else if(!strcmp(p[0],"status\n"))
			{
				//printf("hihi\n");
				index_status(&head);
				//printf("dadada\n");
			}

			k=0;
			while(p[k] != NULL)
			{
				p[k] = NULL;
				k++;
			}
			k =0;
	        
	    }
	    fclose(fp);
	    if(line)
	    	free(line);

	}

	else
	{
		struct node2 *pass;
		struct node2 *head = (struct node2*) malloc(sizeof(struct node2));
		head->ldepth = global_depth;
		head->number = 0;
		head->p = (int*) malloc(bucket_size * sizeof(int));
		head->left = bucket_size;
		head->next = NULL;
		head->over_left = bucket_size;
		head->over = (int*) malloc(bucket_size * sizeof(int));
		if(global_depth > 0)
		{
			for(i=0;i<global_depth;i++)
				double_dir2(&head);
		}

		pass = head->next;
		while(pass != NULL)
		{
			pass->ldepth = global_depth;
			pass->p = (int*) malloc(bucket_size * sizeof(int));
			pass->over = (int*)malloc(bucket_size * sizeof(int));
			pass = pass->next;
		}




		FILE *fp = fopen(argv[4],"r");
		if(fp ==0)
			exit(EXIT_FAILURE);

		while ((read = getline(&line, &len, fp)) != -1) 
		{


			p[0] = strtok(line, " ");
			i = 0;

			while(p[i] != NULL)
			{
				i++;
				p[i] = strtok(NULL," ");
			}

			//printf("%s %s\n",p[0],p[1]);
			if(!strcmp(p[0],"insert"))
			{
				//printf("1\n");
				insert_data2(atoi(p[1]),&head);
			}
			else if (!strcmp(p[0],"delete"))
			{
				delete_12(atoi(p[1]),&head);
			}

			else if(!strcmp(p[0],"search"))
			{
				search2(atoi(p[1]),&head);
			}

			else if(!strcmp(p[0],"status\n"))
			{
				//printf("hihi\n");
				index_status2(&head);
				//printf("dadada\n");
			}

			k=0;
			while(p[k] != NULL)
			{
				p[k] = NULL;
				k++;
			}
			k =0;
	        
	    }
	    fclose(fp);
	    if(line)
	    	free(line);
	}





	return 0;
}