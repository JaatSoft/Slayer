// SettingsWindow.cpp
// Generated by Interface Elements (Window v2.2) on Jan 3 1999
// This is a user written class and will not be overwritten.

#include "SettingsWindow.h"
#include "SlayerApp.h"
#include "MiniSlayer.h"

#include <stdlib.h>

SettingsWindow::SettingsWindow(void)
	: IEWindow("SettingsWindow")
{
	// read options
	if (Lock()) {
		OptionsToDialog();

		// save old options:
		saved_options = slayer->options;

		// if necessary, disable docking button
		if (slayer->docked) {
			BButton *b = (BButton *)FindView("SettingsDeskbar");
			b->SetEnabled(false);
		}
		Unlock();
	}

	Show();
}


SettingsWindow::~SettingsWindow(void)
{
}


// Handling of user interface and other events
void SettingsWindow::MessageReceived(BMessage *message)
{

	switch(message->what){
		case IE_SETTINGSWINDOW_SETTINGSCURRENTWORKSPACE:
			slayer->options.workspace_activation = Options::current_workspace; break;
		case IE_SETTINGSWINDOW_SETTINGSSAVEDWORKSPACE:
			slayer->options.workspace_activation = Options::saved_workspace; break;
		case IE_SETTINGSWINDOW_SETTINGSALLWORKSPACES:
			slayer->options.workspace_activation = Options::all_workspaces; break;
		case IE_SETTINGSWINDOW_SETTINGSSAVEWINDOW:	// 'SettingsSaveWindow' is pressed...
			slayer->options.save_wind_pos = 
				save_window->Value() ? true : false;
			break;

		case IE_SETTINGSWINDOW_SETTINGSSAVEWORKSPACE:	// 'SettingsSaveWorkspace' is pressed...
			slayer->options.save_workspace = 
				save_workspace->Value() ? true : false;

			break;

		case IE_SETTINGSWINDOW_SETTINGSREFRESH:	// Enter is pressed in 'SettingsRefresh'...
			SetRefreshDelay();
			break;
		case IE_SETTINGSWINDOW_SETTINGSREVERT:
			slayer->options = saved_options;
			OptionsToDialog();
			// well this fucker..
			PostMessage(IE_SETTINGSWINDOW_SETTINGSREFRESH);
			break;
		case IE_SETTINGSWINDOW_SETTINGSDESKBAR:
		{
			// Dock to deskbar
			MiniSlayer *replicant =
				new MiniSlayer();
			BMessage archiveMsg(B_ARCHIVED_OBJECT);
			replicant->Archive(&archiveMsg);
			BMessenger messenger("application/x-vnd.Be-TSKB", -1, NULL);
			messenger.SendMessage(&archiveMsg);
			be_app->PostMessage(B_QUIT_REQUESTED);						
		}
			break;
		default:
			inherited::MessageReceived(message);
			break;
	}

}

void SettingsWindow::Quit()
{
	SetRefreshDelay();
	
	IEWindow::Quit();
}

void SettingsWindow::OptionsToDialog()
{
	char str[21];
	sprintf(str, "%g", ((float)slayer->options.refresh)/1000);
	refresh = (BTextControl *)FindView("SettingsRefresh");
	refresh->SetText(str);
	
	save_window = (BCheckBox *)FindView("SettingsSaveWindow");
	if (slayer->options.save_wind_pos)
		save_window->SetValue(1);
	else
		save_window->SetValue(0);
		
	save_workspace = (BCheckBox *)FindView("SettingsSaveWorkspace");
	save_workspace->SetValue(slayer->options.save_workspace ? 1 : 0);
	
	current_workspace = (BRadioButton *)FindView("SettingsCurrentWorkspace");
	all_workspaces = (BRadioButton *)FindView("SettingsAllWorkspaces");
	saved_workspace = (BRadioButton *)FindView("SettingsSavedWorkspace");
	
	switch (slayer->options.workspace_activation) {
	case Options::current_workspace:
		current_workspace->SetValue(1); break;
	case Options::all_workspaces:
		all_workspaces->SetValue(1); break;
	default:
		saved_workspace->SetValue(1); break;
	}
}

void SettingsWindow::SetRefreshDelay()
{
	int32 ref = (int32)(atof(refresh->Text()) * 1000);
	if (ref < 0) ref = 0;
	// lock the window so that it is certain that the threads
	// aren't updating when we do kill
	if (slayer->mainWindow->Lock()) {
		// kill updater
		slayer->mainWindow->refreshThread->Kill();
		slayer->options.refresh = ref;
		// run the updater again
		slayer->mainWindow->refreshThread->Go();
		slayer->mainWindow->Unlock();
	}
}