#include <wx/wx.h>
#include <wx/treectrl.h>
#include <memory>
#include "../Data/DataHelper.h"
#include "../Defines.h"
#include "../Settings.h"
#include "../TreeMenuItemData.h"

class TreeMenu : public wxPanel
{
public:
	TreeMenu(wxWindow *parent, wxWindowID id);
	~TreeMenu();

	void Update();
	std::shared_ptr<Account> GetAccount();

	std::function<void(std::shared_ptr<Account> account)> OnAccountSelect;
	std::function<void(std::shared_ptr<Report> report)> OnReportSelect;
	std::function<void()> OnHomeSelect;
	std::function<void()> OnBudgetsSelect;
	std::function<void(TreeMenuItemTypes type)> OnAccountsSelect;
	std::function<void()> OnAddAccount;
	std::function<void(std::shared_ptr<Account> account)> OnEditAccount;
	std::function<void(std::shared_ptr<Account> account)> OnDeleteAccount;
	std::function<void()> OnAddTransaction;
	std::function<void()> OnNewTab;

private:
	wxTreeCtrl *treeMenu;
	wxImageList *imageList;
	wxImageList *accountsImageList;
	vector<std::shared_ptr<Account>> accounts;
	vector<std::shared_ptr<Report>> reports;
	int selectedAccountId;
	wxTreeItemId selectedMenuItem;

	void CreateImageList();
	void OnTreeSpecItemMenu(wxTreeEvent &event);
	void OnTreeItemSelect(wxTreeEvent &event);
	void OnMenuAddAccount(wxCommandEvent &event);
	void OnMenuEditAccount(wxCommandEvent &event);
	void OnMenuDeleteAccount(wxCommandEvent &event);
	void OnMenuAddTransaction(wxCommandEvent &event);
	void OnOpenNewTab(wxCommandEvent &event);
};

