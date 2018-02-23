// Copyright (c) Athena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

/**
 * Custom Atcommands
 * Place the body of custom atcommands in this file.
 * Format:
 *	ACMD_FUNC(command_name)
 *	{
 *		<code>;
 *	}
 **/

//ACMD_FUNC(newcommand)
//{
//	clif_displaymessage(fd, "It works!");
//	clif_specialeffect(&sd->bl, EF_HEARTCASTING, AREA);
//	return 0;
//}


/*==========================================
 * �̺�Ʈ ���ɾ� [JaVb]
 *------------------------------------------*/
int atcommand_eevent_timer(int tid, unsigned int tick, int id, intptr_t data)
{
	char output[200];

	switch(data) 
	{
		case 1:
			if(event_eflag==1&&event_dflag==0) {
				event_tick-=1;
				if(event_tick<=0) {
					event_tick=0;
					event_tid=0;
					event_rate=0;
					event_eflag=0;
					// �ߴ�
					sprintf(output,"[ ����ġ �̺�Ʈ�� ����Ǿ����ϴ�. ]");
				}
				else {
					sprintf(output,"[ ����ġ %.1f�� �̺�Ʈ�� %d�� ���ҽ��ϴ�. ]", ((event_rate / 10.f) + 1.0), event_tick);
					add_timer(gettick()+60000,atcommand_eevent_timer,event_tid,1);
				}
				intif_broadcast(output, strlen(output) + 1, BC_DEFAULT);
				return 1;
			}
			return 1;

		case 2:
			if(event_dflag==1&&event_eflag==0) {
				event_tick-=1;
				if(event_tick<=0) {
					event_tick=0;
					event_tid=0;
					event_rate=0;
					event_dflag=0;
					// �ߴ�
					sprintf(output,"[ ��ӷ� �̺�Ʈ�� ����Ǿ����ϴ�. ]");
				}
				else {
					sprintf(output,"[ ��ӷ� %.1f�� �̺�Ʈ�� %d�� ���ҽ��ϴ�. ]", ((event_rate / 10.f) + 1.0), event_tick);
					add_timer(gettick()+60000,atcommand_eevent_timer,event_tid,2);
				}
				intif_broadcast(output, strlen(output) + 1, BC_DEFAULT);
				return 1;
			}
			return 1;
		case 3:
			if(event_eflag==1&&event_dflag==1) {
				event_tick-=1;
				if(event_tick<=0) {
					event_tick=0;
					event_tid=0;
					event_rate=0;
					event_eflag=0;
					event_dflag=0;
					// �ߴ�
					sprintf(output,"[ ����ġȹ�淮�� �����۵�ӷ� �̺�Ʈ�� ����Ǿ����ϴ�. ]");
					}
				else {
					sprintf(output,"[ ����ġȹ�淮�� �����۵�ӷ� %.1f�� �̺�Ʈ�� %d�� ���ҽ��ϴ�. ]", ((event_rate / 10.f) + 1.0), event_tick);
					add_timer(gettick()+60000,atcommand_eevent_timer,event_tid,3);
				}
				intif_broadcast(output, strlen(output) + 1, BC_DEFAULT);
			}
			return 1;
	}
	return 0;
}

/*==========================================
 * @�̺�Ʈ command (usage: @�̺�Ʈ %s <time(min)> <rate(10:x1)> <type(1(exp):2(item):3(both))> <state(0(only_view):1(start):2(stop):3(add_timer)>)
 *------------------------------------------*/
ACMD_FUNC(eevent)
{
	char output[200];
	
	int spl,flag=0,rate,tick,itimer;
	double i=0;

	if(!message||!*message) {
		clif_displaymessage(fd,"[�̺�Ʈ]: ������� Ȯ���� �ֽʽÿ�.");
		sprintf(output,"<����: %s �ð�(��) ����(10:1��) �з�(1(����ġ):2(������):3(�Ѵ�)) ����(0(���º���):1(����):2(����):3(�ð��߰�))>",command);
		clif_displaymessage(fd,output);
		return 3;
	}

	// Message ����
	if((sscanf(message,"%d %d %d %d",&tick,&rate,&spl,&flag))<3) {
		clif_displaymessage(fd,"[�̺�Ʈ]: ������� Ȯ���� �ֽʽÿ�.");
		sprintf(output,"<����: %s �ð�(��) ����(10:1��) �з�(1(����ġ):2(������):3(�Ѵ�)) ����(0(���º���):1(����):2(����):3(�ð��߰�))>",command);
		clif_displaymessage(fd,output);
		return 3;
	}
	
	// �� �Էµ� ������ Ȯ��
	if(flag<0||flag>3) {
		clif_displaymessage(fd,"[�̺�Ʈ]: ���۸� Ȯ���� �ֽʽÿ�. ");
		sprintf(output,"<����: %s �ð�(��) ����(10:1��) �з�(1(����ġ):2(������):3(�Ѵ�)) ����(0(���º���):1(����):2(����):3(�ð��߰�))>",command);
		clif_displaymessage(fd,output);
		return 3;
	}
	
	if(spl!=1&&spl!=2&&spl!=3) {
		clif_displaymessage(fd,"[�̺�Ʈ]: �з� �÷��׸� Ȯ���� �ֽʽÿ�. ");
		sprintf(output,"<����: %s �ð�(��) ����(10:1��) �з�(1(����ġ):2(������):3(�Ѵ�)) ����(0(���º���):1(����):2(����):3(�ð��߰�))>",command);
		clif_displaymessage(fd,output);
		return 3;
	}

	switch(spl) {
		case 1: // ����ġ

			switch(flag)
			{

			case 0:
				if(event_dflag==1 && event_eflag==1) {
					clif_displaymessage(fd,"[�̺�Ʈ]: ���� �ٸ� �ɼ����� �̺�Ʈ�� �������Դϴ�. (����ġ + ��ӷ� �̺�Ʈ)");
					return 1;

				} else if(event_dflag==1) {
					clif_displaymessage(fd,"[�̺�Ʈ]: ���� �ٸ� �ɼ����� �̺�Ʈ�� �������Դϴ�. (��ӷ� �̺�Ʈ)");
					return 1;
				}
				// ���� ǥ��
				if(event_eflag==0) {
					sprintf(output,"[�̺�Ʈ]: ���� ����ġ �̺�Ʈ�� �������� �ƴմϴ�.");
				}
				else if(event_eflag==1){
						sprintf(output,"[�̺�Ʈ]: ���� ����ġ �̺�Ʈ�� �������Դϴ�.(���� �ð� %d��)",event_tick);
				}
				else {
					sprintf(output,"[�̺�Ʈ]: ���� �÷��׷� �۵����� �̺�Ʈ�� �ƴմϴ�.");
				}
				clif_displaymessage(fd,output);
				return 2;

			case 1:
				// �̹� flag �۵����ΰ� �ֳ� Ȯ��
				if(event_dflag==1 && event_eflag==1) {
					clif_displaymessage(fd,"[�̺�Ʈ]: ���� �ٸ� �ɼ����� �̺�Ʈ�� �������Դϴ�. (����ġ + ��ӷ� �̺�Ʈ)");
					return 1;

				} else if(event_dflag==1) {
					clif_displaymessage(fd,"[�̺�Ʈ]: ���� �ٸ� �ɼ����� �̺�Ʈ�� �������Դϴ�. (��ӷ� �̺�Ʈ)");
					return 1;
				}
				
				
				// �̹� �������� äũ
				if(event_eflag==1) {
					sprintf(output,"[�̺�Ʈ]: �̹� ����ġ �̺�Ʈ�� �������Դϴ�. �����ð�: %d��",event_tick);
					clif_displaymessage(fd,output);
					return 1;
				}

				// tick äũ 
				if(tick<=0) {
					clif_displaymessage(fd,"[�̺�Ʈ]: �ð� �����Ͱ� �߸��Ǿ����ϴ�.");;
					return 1;
				}

				// rate äũ
				if(rate<=0) {
					clif_displaymessage(fd,"[�̺�Ʈ]: ���� �����Ͱ� �߸��Ǿ����ϴ�.");
					return 1;
				}

				
				// Ȯ�� ����
				event_rate=rate;

				// �÷��� ����
				event_eflag=1;
				
				// �ð� ����
				event_tick=tick;

				// ���� ��Ȯ��
				if(event_eflag==0) {
					// �÷��� �缳��
					clif_displaymessage(fd,"[�̺�Ʈ]: �÷��� �缳�� ����");
					ShowError("atcommand_eevent: Flag Error!\n");
					return 1;
				}

				// Tick ����
				if(event_tick!=tick) {
					// Tick
					ShowError("atcommand_eevent: Timer Tick ���� \n");
					clif_displaymessage(fd,"[�̺�Ʈ]: Timer Tick Error!");
					return 1;
				}

				// Ȯ��
				if(event_rate!=rate) {
					// ����
					clif_displaymessage(fd,"[�̺�Ʈ]: EXP bonus Rate Error!");
					ShowError("atcommand_eevent: EXP bonus Rate Error!\n");
					return 1;
				}
				
				// �������� Timer ���� ����
				itimer = add_timer(gettick()+60000,atcommand_eevent_timer,event_tid,1);

				event_tid = itimer;

				// ��� ����
				i=((double)event_rate/10)+1;

				sprintf(output,"[ %d�а� ����ġ ȹ�淮�� %.1f�谡 �˴ϴ�. ]",event_tick,i);
				clif_displaymessage(fd,output);
				intif_broadcast(output, strlen(output) + 1, BC_DEFAULT);
				clif_displaymessage(fd,"[�̺�Ʈ]: ����ġ �̺�Ʈ�� ���۵Ǿ����ϴ�.");
				return 0;

			case 2:
				// �ߴ�
				if(event_dflag==1 && event_eflag==1) {
					clif_displaymessage(fd,"[�̺�Ʈ]: ���� �ٸ� �ɼ����� �̺�Ʈ�� �������Դϴ�. (����ġ + ��ӷ� �̺�Ʈ)");
					return 1;

				} else if(event_dflag==1) {
					clif_displaymessage(fd,"[�̺�Ʈ]: ���� �ٸ� �ɼ����� �̺�Ʈ�� �������Դϴ�. (��ӷ� �̺�Ʈ)");
					return 1;
				}

				if(event_eflag==0) {
					clif_displaymessage(fd,"[�̺�Ʈ]: ����ġ �̺�Ʈ�� �������� �ƴմϴ�.");
					return 1;
				}
				
				// ���� Timer ����
				delete_timer(event_tid,atcommand_eevent_timer);
				
				// �� ������
				event_eflag=0;
				event_rate=0;
				event_tick=0;
				event_tid=0;

				if(event_eflag!=0) {
					ShowError("atcommand_eevent: Flag ��ü ����\n");
					clif_displaymessage(fd,"atcommand_eevent: Flag ��ü ����");
					return 1;
				}
				
				if(event_rate!=0) {
					ShowError("atcommand_eevent: Ȯ�� ���� ����\n");
					clif_displaymessage(fd,"atcommand_eevent: Ȯ�� ���� ����");
					return 1;
				}

				if(event_tick!=0) {
					ShowError("atcommand_eevent: ƽ ���� ����\n");
					clif_displaymessage(fd,"atcommand_eevent: ƽ ���� ����");
					return 1;
				}

				if(event_tid!=0) {
					ShowError("atcommand_eevent: Ÿ�̸� ���̵� ���� ����\n");
					clif_displaymessage(fd,"atcommand_eevent: Ÿ�̸� ���̵� ���� ����");
					return 1;
				}
				
				// ��... ���� ���
				intif_broadcast("[ ����ġ �̺�Ʈ�� ����Ǿ����ϴ�. ]", strlen("[ ����ġ �̺�Ʈ�� ����Ǿ����ϴ�. ]") + 1, BC_DEFAULT);
				clif_displaymessage(fd,"[�̺�Ʈ]: ����ġ �̺�Ʈ�� ���� �Ǿ����ϴ�.");
				return 0;

			case 3:
				if(event_dflag==1 && event_eflag==1) {
					clif_displaymessage(fd,"[�̺�Ʈ]: ���� �ٸ� �ɼ����� �̺�Ʈ�� �������Դϴ�. (����ġ + ��ӷ� �̺�Ʈ)");
					return 1;

				} else if(event_dflag==1) {
					clif_displaymessage(fd,"[�̺�Ʈ]: ���� �ٸ� �ɼ����� �̺�Ʈ�� �������Դϴ�. (��ӷ� �̺�Ʈ)");
					return 1;
				}
				// Ÿ�� �߰�.
				if(event_eflag==1) {
					// �߰� ����
					if(tick<0) {
						if(tick<(event_tick*-1)) {
							tick=(event_tick*-1);
							event_tick=1;
						}
						else {
							event_tick+=tick;
						}
					} else {
						event_tick+=tick;
					}


					// ��� ����.
					sprintf(output,"[ ����ġ �̺�Ʈ�� %d�� ����Ǿ� %d�� ���ҽ��ϴ�. ]",tick,event_tick);
					intif_broadcast(output, strlen(output) + 1, BC_DEFAULT);
					clif_displaymessage(fd,"[�̺�Ʈ]: ����ġ �̺�Ʈ�� ���������� ����Ǿ����ϴ�.");
					return 1;
				}
				else {
					clif_displaymessage(fd,"[�̺�Ʈ]: ����ġ �̺�Ʈ�� �������� �ƴմϴ�.");
					return 1;
				}
					
			default:
				return false;
			}
		
		case 2:
			// ������
			switch(flag)
			{

			case 0:
				if(event_dflag==1 && event_eflag==1) {
					clif_displaymessage(fd,"[�̺�Ʈ]: ���� �ٸ� �ɼ����� �̺�Ʈ�� �������Դϴ�. (����ġ + ��ӷ� �̺�Ʈ)");
					return 1;

				} else if(event_eflag==1) {
					clif_displaymessage(fd,"[�̺�Ʈ]: ���� �ٸ� �ɼ����� �̺�Ʈ�� �������Դϴ�. (����ġ �̺�Ʈ)");
					return 1;
				}
				if(event_dflag==0) {
					sprintf(output,"[�̺�Ʈ]: ���� ��ӷ� �̺�Ʈ�� �������� �ƴմϴ�.");
				}
				else if(event_dflag==1){
						sprintf(output,"[�̺�Ʈ]: ���� ��ӷ� �̺�Ʈ�� �������Դϴ�.(���� �ð� %d��)",event_tick);
				}
				else {
					sprintf(output,"[�̺�Ʈ]: ���� �÷��׷� �۵����� �̺�Ʈ�� �ƴմϴ�.");
				}
				clif_displaymessage(fd,output);
				return 2;

			case 1:
				// �̹� flag �۵����ΰ� �ֳ� Ȯ��
				if(event_dflag==1 && event_eflag==1) {
					clif_displaymessage(fd,"[�̺�Ʈ]: ���� �ٸ� �ɼ����� �̺�Ʈ�� �������Դϴ�. (����ġ + ��ӷ� �̺�Ʈ)");
					return 1;

				} else if(event_eflag==1) {
					clif_displaymessage(fd,"[�̺�Ʈ]: ���� �ٸ� �ɼ����� �̺�Ʈ�� �������Դϴ�. (����ġ �̺�Ʈ)");
					return 1;
				}
				
				
				// �̹� �������� äũ
				if(event_dflag==1) {
					sprintf(output,"[�̺�Ʈ]: �̹� ��ӷ� �̺�Ʈ�� �������Դϴ�. �����ð�: %d��",event_tick);
					clif_displaymessage(fd,output);
					return 1;
				}

				// tick äũ 
				if(tick<=0) {
					clif_displaymessage(fd,"[�̺�Ʈ]: �ð� �����Ͱ� �߸��Ǿ����ϴ�.");;
					return 1;
				}

				// rate äũ
				if(rate<=0) {
					clif_displaymessage(fd,"[�̺�Ʈ]: ���� �����Ͱ� �߸��Ǿ����ϴ�.");
					return 1;
				}

				
				// Ȯ�� ����
				event_rate=rate;

				// �÷��� ����
				event_dflag=1;
				
				// �ð� ����
				event_tick=tick;

				// ���� ��Ȯ��
				if(event_dflag==0) {
					// �÷��� �缳��
					clif_displaymessage(fd,"[�̺�Ʈ]: �÷��� �缳�� ����");
					ShowError("atcommand_eevent: Flag Error!\n");
					return 1;
				}

				// Tick ����
				if(event_tick!=tick) {
					// Tick
					ShowError("atcommand_eevent: Timer Tick ���� \n");
					clif_displaymessage(fd,"[�̺�Ʈ]: Timer Tick Error!");
					return 1;
				}

				// Ȯ��
				if(event_rate!=rate) {
					// ����
					clif_displaymessage(fd,"[�̺�Ʈ]: Drop rate bonus Rate Error!");
					ShowError("atcommand_eevent: Drop rate bonus Rate Error!\n");
					return 1;
				}
				
					// �������� Timer ���� ����
				itimer = add_timer(gettick()+60000,atcommand_eevent_timer,event_tid,2);
				event_tid = itimer;

				// ��� ����
				i=((double)event_rate/10)+1;

				sprintf(output,"[ %d�а� ������ ��ӷ��� %.1f�谡 �˴ϴ�. ]",event_tick,i);
				clif_displaymessage(fd,output);
				intif_broadcast(output, strlen(output) + 1, BC_DEFAULT);
				clif_displaymessage(fd,"[�̺�Ʈ]: ��ӷ� �̺�Ʈ�� ���۵Ǿ����ϴ�.");
				return 0;

			case 2:
				// �ߴ�
				if(event_dflag==1 && event_eflag==1) {
					clif_displaymessage(fd,"[�̺�Ʈ]: ���� �ٸ� �ɼ����� �̺�Ʈ�� �������Դϴ�. (����ġ + ��ӷ� �̺�Ʈ)");
					return 1;

				} else if(event_eflag==1) {
					clif_displaymessage(fd,"[�̺�Ʈ]: ���� �ٸ� �ɼ����� �̺�Ʈ�� �������Դϴ�. (����ġ �̺�Ʈ)");
					return 1;
				}
				
				if(event_dflag==0) {
					clif_displaymessage(fd,"[�̺�Ʈ]: ��ӷ� �̺�Ʈ�� �������� �ƴմϴ�.");
					return 2;
				}
				
				// ���� Timer ����
				delete_timer(event_tid,atcommand_eevent_timer);
				
				// �� ������
				event_dflag=0;
				event_rate=0;
				event_tick=0;
				event_tid=0;

				if(event_dflag!=0) {
					ShowError("atcommand_eevent: Flag ��ü ����\n");
					clif_displaymessage(fd,"atcommand_eevent: Flag ��ü ����");
					return 1;
				}
				
				if(event_rate!=0) {
					ShowError("atcommand_eevent: Ȯ�� ���� ����\n");
					clif_displaymessage(fd,"atcommand_eevent: Ȯ�� ���� ����");
					return 1;
				}

				if(event_tick!=0) {
					ShowError("atcommand_eevent: ƽ ���� ����\n");
					clif_displaymessage(fd,"atcommand_eevent: ƽ ���� ����");
					return 1;
				}

				if(event_tid!=0) {
					ShowError("atcommand_eevent: Ÿ�̸� ���̵� ���� ����\n");
					clif_displaymessage(fd,"atcommand_eevent: Ÿ�̸� ���̵� ���� ����");
					return 1;
				}
				
				// ��... ���� ���
				sprintf(output,"[ ��ӷ� �̺�Ʈ�� ����Ǿ����ϴ�. ]");
				intif_broadcast(output, strlen(output) + 1, BC_DEFAULT);
				clif_displaymessage(fd,"[�̺�Ʈ]: ��ӷ� �̺�Ʈ�� ���� �Ǿ����ϴ�.");
				return 0;

			case 3:
				// Ÿ�� �߰�.
				if(event_dflag==1 && event_eflag==1) {
					clif_displaymessage(fd,"[�̺�Ʈ]: ���� �ٸ� �ɼ����� �̺�Ʈ�� �������Դϴ�. (����ġ + ��ӷ� �̺�Ʈ)");
					return 1;

				} else if(event_eflag==1) {
					clif_displaymessage(fd,"[�̺�Ʈ]: ���� �ٸ� �ɼ����� �̺�Ʈ�� �������Դϴ�. (����ġ �̺�Ʈ)");
					return 1;
				}
				if(event_dflag==1) {
					// �߰� ����
					if(tick<0) {
						if(tick<(event_tick*-1)) {
							tick=(event_tick*-1);
							event_tick=1;
						}
						else {
							event_tick+=tick;
						}
					} else {
						event_tick+=tick;
					}


					// ��� ����.
					sprintf(output,"[ ��ӷ� �̺�Ʈ�� %d�� ����Ǿ� %d�� ���ҽ��ϴ�. ]",tick,event_tick);
					intif_broadcast(output, strlen(output) + 1, BC_DEFAULT);
					clif_displaymessage(fd,"[�̺�Ʈ]: ��ӷ� �̺�Ʈ�� ���������� ����Ǿ����ϴ�.");
					return 0;
				}
				else {
					clif_displaymessage(fd,"[�̺�Ʈ]: ��ӷ� �̺�Ʈ�� �������� �ƴմϴ�.");
					return 1;
				}
					
			default:
				return 1;
			}

		case 3: // ����
			switch(flag)
			{

			case 0:
				if(event_eflag!=event_dflag) {
					if(event_eflag==1) {
						clif_displaymessage(fd,"[�̺�Ʈ]: ���� �ٸ� �ɼ����� �̺�Ʈ�� �������Դϴ�. (����ġ �̺�Ʈ)");
						return 1;

					} else if(event_dflag==1) {
						clif_displaymessage(fd,"[�̺�Ʈ]: ���� �ٸ� �ɼ����� �̺�Ʈ�� �������Դϴ�. (��ӷ� �̺�Ʈ)");
						return 1;
					}
				}
				// ���� ǥ��
				if(event_eflag==0 && event_dflag==0) {
					sprintf(output,"[�̺�Ʈ]: ���� ���� �̺�Ʈ �������� �ƴմϴ�.");
				}
				else if(event_eflag==1 && event_dflag==1){
						sprintf(output,"[�̺�Ʈ]: ���� ���� �̺�Ʈ�� �������Դϴ�.(���� �ð� %d��)",event_tick);
				}
				else {
					sprintf(output,"[�̺�Ʈ]: ���� �÷��׷� �۵����� �̺�Ʈ�� �ƴմϴ�.");
				}
				clif_displaymessage(fd,output);
				return 1;

			case 1:
				// �̹� flag �۵����ΰ� �ֳ� Ȯ��
				if(event_eflag==1) {
					clif_displaymessage(fd,"[�̺�Ʈ]: ���� �ٸ� �ɼ����� �̺�Ʈ�� �������Դϴ�. (����ġ �̺�Ʈ)");
					return 1;

				} else if(event_dflag==1) {
					clif_displaymessage(fd,"[�̺�Ʈ]: ���� �ٸ� �ɼ����� �̺�Ʈ�� �������Դϴ�. (��ӷ� �̺�Ʈ)");
					return 1;
				}
				
				
				// �̹� �������� äũ
				if(event_eflag==1&&event_dflag==1) {
					sprintf(output,"[�̺�Ʈ]: �̹� ���� �̺�Ʈ�� �������Դϴ�. �����ð�: %d��",event_tick);
					clif_displaymessage(fd,output);
					return 1;
				}

				// tick äũ 
				if(tick<=0) {
					clif_displaymessage(fd,"[�̺�Ʈ]: �ð� �����Ͱ� �߸��Ǿ����ϴ�.");;
					return 1;
				}

				// rate äũ
				if(rate<=0) {
					clif_displaymessage(fd,"[�̺�Ʈ]: ���� �����Ͱ� �߸��Ǿ����ϴ�.");
					return 1;
				}

				
				// Ȯ�� ����
				event_rate=rate;

				// �÷��� ����
				event_eflag=1;
				event_dflag=1;

				// �ð� ����
				event_tick=tick;

				// ���� ��Ȯ��
				if(event_eflag==0||event_dflag==0) {
					// �÷��� �缳��
					clif_displaymessage(fd,"[�̺�Ʈ]: �÷��� �缳�� ����");
					ShowError("atcommand_eevent: Flag Error!\n");
					return 1;
				}

				// Tick ����
				if(event_tick!=tick) {
					// Tick
					ShowError("atcommand_eevent: Timer Tick ���� \n");
					clif_displaymessage(fd,"[�̺�Ʈ]: Timer Tick Error!");
					return 1;
				}

				// Ȯ��
				if(event_rate!=rate) {
					// ����
					clif_displaymessage(fd,"[�̺�Ʈ]: Total bonus Rate Error!");
					ShowError("atcommand_eevent: Total bonus Rate Error!\n");
					return 1;
				}
		
				itimer = add_timer(gettick()+60000,atcommand_eevent_timer,event_tid,3);

				event_tid = itimer;

				// ��� ����
				i=((double)event_rate/10)+1;

				sprintf(output,"[ %d�а� ��ӷ��� ����ġ���� %.1f�谡 �˴ϴ�. ]",event_tick,i);
				clif_displaymessage(fd,output);
				intif_broadcast(output, strlen(output) + 1, BC_DEFAULT);
				clif_displaymessage(fd,"[�̺�Ʈ]: ���� �̺�Ʈ�� ���۵Ǿ����ϴ�.");
				return 0;

			case 2:
				// �ߴ�
				if(event_eflag!=event_dflag) {
					if(event_eflag==1) {
						clif_displaymessage(fd,"[�̺�Ʈ]: ���� �ٸ� �ɼ����� �̺�Ʈ�� �������Դϴ�. (����ġ �̺�Ʈ)");
						return 1;

					} else if(event_dflag==1) {
						clif_displaymessage(fd,"[�̺�Ʈ]: ���� �ٸ� �ɼ����� �̺�Ʈ�� �������Դϴ�. (��ӷ� �̺�Ʈ)");
						return 1;
					}
				}

				if(event_dflag==0&&event_eflag==0) {
					clif_displaymessage(fd,"[�̺�Ʈ]: ���� �̺�Ʈ�� �������� �ƴմϴ�.");
					return 1;
				}

				delete_timer(event_tid,atcommand_eevent_timer);

				
				// �� ������
				event_eflag=0;
				event_dflag=0;
				event_rate=0;
				event_tick=0;
				event_tid=0;

				if(event_eflag!=0||event_dflag!=0) {
					ShowError("atcommand_eevent: Flag ��ü ����\n");
					clif_displaymessage(fd,"atcommand_eevent: Flag ��ü ����");
					return 1;
				}
				
				if(event_rate!=0) {
					ShowError("atcommand_eevent: Ȯ�� ���� ����\n");
					clif_displaymessage(fd,"atcommand_eevent: Ȯ�� ���� ����");
					return 1;
				}

				if(event_tick!=0) {
					ShowError("atcommand_eevent: ƽ ���� ����\n");
					clif_displaymessage(fd,"atcommand_eevent: ƽ ���� ����");
					return 1;
				}

				if(event_tid!=0) {
					ShowError("atcommand_eevent: Ÿ�̸� ���̵� ���� ����\n");
					clif_displaymessage(fd,"atcommand_eevent: Ÿ�̸� ���̵� ���� ����");
					return 1;
				}
				
				// ��... ���� ���
				sprintf(output,"[ ���� �̺�Ʈ�� ����Ǿ����ϴ�. ]");
				intif_broadcast(output, strlen(output) + 1, BC_DEFAULT);
				clif_displaymessage(fd,"[�̺�Ʈ]: ���� �̺�Ʈ�� ���� �Ǿ����ϴ�.");
				return 0;

			case 3:
				if(event_eflag!=event_dflag) {
					if(event_eflag==1) {
						clif_displaymessage(fd,"[�̺�Ʈ]: ���� �ٸ� �ɼ����� �̺�Ʈ�� �������Դϴ�. (����ġ �̺�Ʈ)");
						return 1;

					} else if(event_dflag==1) {
						clif_displaymessage(fd,"[�̺�Ʈ]: ���� �ٸ� �ɼ����� �̺�Ʈ�� �������Դϴ�. (��ӷ� �̺�Ʈ)");
						return 1;
					}
				}
				// Ÿ�� �߰�.
				if(event_eflag==1&&event_dflag==1) {
					// �߰� ����
					if(tick<0) {
						if(tick<(event_tick*-1)) {
							tick=(event_tick*-1);
							event_tick=1;
						}
						else {
							event_tick+=tick;
						}
					} else {
						event_tick+=tick;
					}


					// ��� ����.
					sprintf(output,"[ ���� �̺�Ʈ�� %d�� ����Ǿ� %d�� ���ҽ��ϴ�. ]",tick,event_tick);
					intif_broadcast(output, strlen(output) + 1, BC_DEFAULT);
					clif_displaymessage(fd,"[�̺�Ʈ]: ���� �̺�Ʈ�� ���������� ����Ǿ����ϴ�.");
					return 0;
				}
				else {
					clif_displaymessage(fd,"[�̺�Ʈ]: ���� �̺�Ʈ�� �������� �ƴմϴ�.");
					return 1;
				}
					
			default:
				return 1;
			}

		default:
			return 1;

	}

	return 1;
}

static void atcommand_buf_sub(const int fd,struct map_session_data* sd,const int mode)
{
	if (sd && sd->state.active) 
	{
		int time;

		switch(mode)
		{
			case 1:	time=10*60;	break;
			case 2:	time=15*60;	break;
			case 3:	time=20*60;	break;
			default:time=10*60;	break;
		}

		switch(mode)
		{
			case 3:
				if(pc_isdead(sd)) 
				{
					clif_skill_nodamage(&sd->bl,&sd->bl,ALL_RESURRECTION,4,1);
					status_revive(&sd->bl, 100, 100);
				}
			case 2:
				clif_skill_nodamage(&sd->bl,&sd->bl,PR_MAGNIFICAT,10,sc_start(&sd->bl,&sd->bl,SC_MAGNIFICAT,100,10,time*1000));
				clif_skill_nodamage(&sd->bl,&sd->bl,PR_GLORIA,10,sc_start(&sd->bl,&sd->bl,SC_GLORIA,100,10,time*1000));
			case 1:
				clif_skill_nodamage(&sd->bl,&sd->bl,HP_ASSUMPTIO,10,sc_start(&sd->bl,&sd->bl,SC_ASSUMPTIO,100,10,time*1000));
				clif_skill_nodamage(&sd->bl,&sd->bl,PR_IMPOSITIO,10,sc_start(&sd->bl,&sd->bl,SC_IMPOSITIO,100,10,time*1000));
			default:
				clif_skill_nodamage(&sd->bl,&sd->bl,AL_BLESSING,10,sc_start(&sd->bl,&sd->bl,SC_BLESSING,100,10,time*1000));
				clif_skill_nodamage(&sd->bl,&sd->bl,AL_INCAGI,10,sc_start(&sd->bl,&sd->bl,SC_INCREASEAGI,100,10,time*1000));
		}

		if (battle_config.pc_invincible_time > 0)
			pc_setinvincibletimer(sd, battle_config.pc_invincible_time);
	}
}

ACMD_FUNC(bbuf)
{
	int mode = 0;
	struct s_mapiterator* iter;
	struct map_session_data* pl_sd;

	char temp[1024];
	char msg[50];

	nullpo_retr(-1, sd);

	if(!message || !*message || sscanf(message, "%d", &mode) < 1) 
	{
		clif_displaymessage(fd, "���� ��ȣ�� ������ �ֽʽÿ�. (����: @���� <��ȣ, �޽���>)");
		clif_displaymessage(fd, " 0 : ������,��ø������(10��)");
		clif_displaymessage(fd, " 1 : ������,��ø������,�Ƽ���Ƽ��,������Ƽ��(10��)");
		clif_displaymessage(fd, " 2 : ������,��ø������,�Ƽ���Ƽ��,������Ƽ��,������ı,�۷θ���(15��)");
		clif_displaymessage(fd, " 3 : ��������,������,��/��,�Ƽ�,����,������ı,�۷θ���(20��)");
		return -1;
	}

	if (mode > 3) mode = 3;

	iter = mapit_getallusers();

	for( pl_sd = (TBL_PC*)mapit_first(iter); mapit_exists(iter); pl_sd = (TBL_PC*)mapit_next(iter) )
		atcommand_buf_sub(fd,pl_sd,mode);

	mapit_free(iter);

	switch(mode)
	{
		case 0: strcpy(msg,"�ణ��");	break;
		case 1:	strcpy(msg,"����");		break;
		case 2:	strcpy(msg,"������");	break;
		case 3:	strcpy(msg,"������");	break;
	}

	sprintf(temp, "<%s>�Բ��� %s �Ŀ��� �����ݴϴ�.", sd->status.name, msg);

	intif_broadcast(temp,strlen(temp)+1,0x10);

	return 0;
}