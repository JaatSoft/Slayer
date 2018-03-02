/**********************************************************************
 * Copyright 1999-2013 Arto Jalkanen
 *
 * This file is part of Slayer.
 *
 * Slayer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Slayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Slayer.  If not, see <http://www.gnu.org/licenses/>
**/
#include "ThreadItem.h"

#include "SlayerApp.h"
#include "Options.h"

#include <ColumnTypes.h>
ThreadItem::ThreadItem() : BRow() {}

ThreadItem::ThreadItem(thread_info *info) : BRow()
{
	thread = info->thread;
	team = info->team;
	state = info->state;
	priority = info->priority;
	sem = info->sem;
	user_time = info->user_time;
	kernel_time = info->kernel_time;
	CPU_diff = user_time + kernel_time;
	CPU = 0.0;
	strcpy(name, info->name);

	char str[21], *strp;
/*	SetColumnContent(TeamListView::name_ndx, name,false);
	sprintf(str, "%ld", thread);
	SetColumnContent(TeamListView::id_ndx, str,false);
	sprintf(str, "%ld", priority);
	SetColumnContent(TeamListView::priority_ndx, str,false);*/
	switch (state) {
	case B_THREAD_RUNNING:
		strp = "Running"; break;
	case B_THREAD_READY:
		strp = "Ready"; break;
	case B_THREAD_SUSPENDED:
		strp = "Suspended"; break;
	case B_THREAD_WAITING:
		strp = "Waiting"; break;
	case B_THREAD_RECEIVING:
		strp = "Receiving"; break;
	case B_THREAD_ASLEEP:
		strp = "Sleeping"; break;
	default:
		strp = "Undefined"; break;
	}
/*	SetColumnContent(TeamListView::state_ndx, strp, false);
	SetColumnContent(TeamListView::areas_ndx, "-", false);

	SetColumnContent(TeamListView::CPU_ndx, "-", false);
*/
	int32 i = 0;
	SetField(new BBitmapField(NULL), i++);
	SetField(new BStringField(name), i++);
	sprintf(str, "%ld", thread);
	SetField(new BStringField(str), i++);
	sprintf(str, "%ld", priority);
	SetField(new BStringField(str), i++);
	SetField(new BStringField(strp), i++);
	SetField(new BSizeField(0), i++);
	SetField(new BIntegerField(0), i++);

	changed = 0;
}

void ThreadItem::update(thread_info *info)
{
	char str[21], *strp;
	CPU_diff = (info->user_time-user_time)+(info->kernel_time-kernel_time);
	user_time = info->user_time;
	kernel_time = info->kernel_time;

	if (strcmp(info->name, name) &&
	    (slayer->options.shown_columns & Options::name_col)) {

		strcpy(name, info->name);
//		SetField(new BStringField(name), 1);
		((BStringField*)GetField(1))->SetString(name);
		changed |= name_chg;
	}
	if ((priority != info->priority) &&
		(slayer->options.shown_columns & Options::priority_col)) {

		priority = info->priority;
		sprintf(str, "%ld", priority);
		//SetColumnContent(TeamListView::priority_ndx, str, false);
		////SetField(new BStringField(str), 3);
		((BStringField*)GetField(3))->SetString(str);
		changed |= priority_chg;
	}
	if ((state != info->state) &&
	    (slayer->options.shown_columns & Options::state_col)) {

		state = info->state;

		switch (state) {
		case B_THREAD_RUNNING:
			strp = "Running"; break;
		case B_THREAD_READY:
			strp = "Ready"; break;
		case B_THREAD_SUSPENDED:
			strp = "Suspended"; break;
		case B_THREAD_WAITING:
			strp = "Waiting"; break;
		case B_THREAD_RECEIVING:
			strp = "Receiving"; break;
		case B_THREAD_ASLEEP:
			strp = "Sleeping"; break;
		default:
			strp = "Undefined"; break;
		}
		//SetColumnContent(TeamListView::state_ndx, strp, false);
		SetField(new BStringField(strp), 4);
		((BStringField*)GetField(4))->SetString(strp);
		changed |= state_chg;
	}
	//SetField(new BIntegerField(CPU * 100.0), 6);
//TODO	//((BIntegerField*)GetField(6))->SetValue(CPU*100);
}

void ThreadItem::DrawItemColumn(BView *owner, BRect itemColumnRect, int32
		columnIndex, bool complete)
{
/*
	if (columnIndex != TeamListView::CPU_ndx)
		return CLVEasyItem::DrawItemColumn(owner, itemColumnRect, columnIndex,
			complete);

	BRect colRect = ItemColumnFrame(columnIndex, (ColumnListView *)owner);
	float sright = colRect.right;
	colRect.bottom -= 1.0;
	colRect.right = colRect.left + (colRect.right - colRect.left) * CPU;
	owner->SetHighColor(200 * CPU, 200 * (1.0 - CPU), 0);
	owner->FillRect(colRect & itemColumnRect);
	if (complete) {
		owner->SetHighColor(255, 255, 255);
		owner->FillRect(BRect(colRect.right + 1.0, colRect.top, sright, colRect.bottom) &
			itemColumnRect);
	}
*/
}
