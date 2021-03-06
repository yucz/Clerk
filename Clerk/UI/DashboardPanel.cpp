#include "DashboardPanel.h"

DashboardPanel::DashboardPanel(wxWindow *parent, wxWindowID id) : DataPanel(parent, id) {
	this->Connect(wxEVT_PAINT, wxPaintEventHandler(DashboardPanel::OnPaint));

	paddingX = 20;
	paddingY = 20;

	color0 = wxColor(165, 210, 75);
	color50 = wxColor(250, 210, 50);
	color100 = wxColor(185, 25, 0);
}

DashboardPanel::~DashboardPanel() {
}

void DashboardPanel::Update() {
	wxDateTime fromDate = wxDateTime::Now();
	wxDateTime toDate = wxDateTime::Now();

	fromDate.SetDay(1);
	toDate.SetToLastMonthDay();

	balance.clear();
	expenses.clear();
	budgets.clear();
	credits.clear();

	for each (auto account in DataHelper::GetInstance().GetAccounts(AccountTypes::Deposit))
	{
		float amount = DataHelper::GetInstance().GetBalance(account.get());

		if (account->creditLimit > 0) {
			float currentAmount = account->creditLimit + amount;
			float remainPercent = abs(currentAmount / account->creditLimit) * 100.0;

			credits.push_back({ *account->name, wxString::Format("%.2f", account->creditLimit), wxString::Format("%.2f", currentAmount),  wxString::Format("%.2f", amount), remainPercent });
		}
		else {
			balance.push_back({ *account->name, wxString::Format("%.2f", amount) });
		}
	}

	for each (auto account in DataHelper::GetInstance().GetAccounts(AccountTypes::Credit))
	{
		float remainAmount = abs(DataHelper::GetInstance().GetBalance(account.get()));
		float amount = abs(DataHelper::GetInstance().GetAccountTotalExpense(account.get()));
		float currentAmount = DataHelper::GetInstance().GetAccountTotalReceipt(account.get());

		float remainPercent = (currentAmount / amount) * 100.0;

		credits.push_back({ *account->name, wxString::Format("%.2f", amount), wxString::Format("%.2f", currentAmount),  wxString::Format("%.2f", remainAmount), remainPercent });
	}

	vector<StringValue> values = DataHelper::GetInstance().GetExpensesByAccount(&fromDate, &toDate);

	sort(values.begin(), values.end(), [](StringValue a, StringValue b) {
		return a.value > b.value;
	});

	for each (auto value in values) {
		expenses.push_back({ value.string, wxString::Format("%.2f", value.value) });
	}

	for each (auto budget in DataHelper::GetInstance().GetBudgets())
	{
		float currentAmount = 0.0;
		wxDateTime toDate = wxDateTime::Now();
		wxDateTime fromDate = wxDateTime::Now();

		if (budget->period == BudgetPeriods::Month) {
			fromDate.SetDay(1);
		}

		if (budget->type == BudgetTypes::Limit) {
			if (!budget->account) {
				currentAmount = DataHelper::GetInstance().GetExpenses(&fromDate, &toDate);
			}
			else {
				currentAmount = DataHelper::GetInstance().GetExpensesSumForAccount(budget->account.get(), &fromDate, &toDate);
			}
		}

		float remainAmount = budget->amount - currentAmount;
		float remainPercent = (currentAmount / budget->amount) * 100.0;

		budgets.push_back({ *budget->name, wxString::Format("%.2f", budget->amount), wxString::Format("%.2f", currentAmount),  wxString::Format("%.2f", remainAmount), remainPercent });
	}

	Draw();
}

void DashboardPanel::Draw() {
	wxClientDC dc(this);

	int width = 0;
	int height = 0;

	this->DoGetSize(&width, &height);

	dc.SetBackground(wxColor(255, 255, 255));
	dc.Clear();

	wxFont titleFont = this->GetFont();
	titleFont.SetPointSize(12);
	titleFont.SetWeight(wxFONTWEIGHT_BOLD);

	wxFont accountFont = this->GetFont();

	wxFont amountFont = this->GetFont();
	amountFont.SetWeight(wxFONTWEIGHT_BOLD);

	wxFont budgetFont = this->GetFont();
	budgetFont.SetPointSize(8);

	int progressWidth = 250;

	dc.SetTextForeground(wxColor(0, 0, 0));
	dc.SetFont(titleFont);

	int y = paddingY + 10;
	int x = paddingX + 10;

	dc.DrawText("Balance", wxPoint(x, y));
	
	y = paddingY + 50;
	
	for each (auto account in balance) {
		dc.SetFont(accountFont);
		dc.SetTextForeground(wxColor(0, 0, 0));
		dc.DrawText(account.name, wxPoint(x, y));

		dc.SetFont(amountFont);
		dc.SetTextForeground(wxColor(60, 60, 60));

		wxSize size = dc.GetTextExtent(account.value);

		dc.DrawText(account.value, wxPoint(x + 250 - size.GetWidth(), y));

		y = y + 20;
	}

	y = y + 20;

	dc.SetTextForeground(wxColor(0, 0, 0));
	dc.SetFont(titleFont);

	dc.DrawText("Credits", wxPoint(x, y));

	y = y + 40;

	for each (auto account in credits) {
		dc.SetFont(accountFont);
		dc.SetTextForeground(wxColor(0, 0, 0));
		dc.DrawText(account.name, wxPoint(x, y));

		dc.SetFont(amountFont);
		dc.SetTextForeground(wxColor(60, 60, 60));

		wxSize size = dc.GetTextExtent(account.remainAmount);

		dc.DrawText(account.remainAmount, wxPoint(x + progressWidth - size.GetWidth(), y));

		y = y + 23;

		dc.SetPen(wxPen(wxColor(200, 200, 200), 3));
		dc.DrawLine(x, y, x + progressWidth, y);

		int width = (progressWidth / 100.0) * account.percent;

		if (account.percent <= 50) {
			dc.SetPen(wxPen(color100, 3));
		}
		else if (account.percent > 50 && account.percent <= 80) {
			dc.SetPen(wxPen(color50, 3));
		}
		else {
			dc.SetPen(wxPen(color0, 3));
		}

		dc.DrawLine(x, y, x + width, y);

		y = y + 5;

		dc.SetFont(budgetFont);
		dc.SetTextForeground(wxColor(60, 60, 60));
		dc.DrawText(account.currentAmount, wxPoint(x, y));

		size = dc.GetTextExtent(account.amount);

		dc.SetFont(budgetFont);
		dc.SetTextForeground(wxColor(120, 120, 120));
		dc.DrawText(account.amount, wxPoint(x + progressWidth - size.GetWidth(), y));

		y = y + 40;
	}

	dc.SetTextForeground(wxColor(0, 0, 0));
	dc.SetFont(titleFont);

	y = paddingY + 10;
	x = width / 2;

	dc.DrawText("Expenses", wxPoint(x, y));

	y = paddingY + 50;

	for each (auto account in expenses) {
		dc.SetFont(accountFont);
		dc.SetTextForeground(wxColor(0, 0, 0));
		dc.DrawText(account.name, wxPoint(x, y));

		dc.SetFont(amountFont);
		dc.SetTextForeground(wxColor(60, 60, 60));

		wxSize size = dc.GetTextExtent(account.value);

		dc.DrawText(account.value, wxPoint(x + 250 - size.GetWidth(), y));

		y = y + 20;
	}

	dc.SetFont(titleFont);

	y = y + 20;

	dc.DrawText("Budgets", wxPoint(x, y));

	y = y + 40;

	for each (auto budget in budgets) {
		dc.SetFont(accountFont);
		dc.SetTextForeground(wxColor(0, 0, 0));
		dc.DrawText(budget.name, wxPoint(x, y));

		dc.SetFont(amountFont);
		dc.SetTextForeground(wxColor(60, 60, 60));

		wxSize size = dc.GetTextExtent(budget.remainAmount);

		dc.DrawText(budget.remainAmount, wxPoint(x + progressWidth - size.GetWidth(), y));

		y = y + 23;
	
		dc.SetPen(wxPen(wxColor(200, 200, 200), 3));
		dc.DrawLine(x, y, x + progressWidth, y);

		int width = (progressWidth / 100.0) * budget.percent;

		if (budget.percent <= 50) {
			dc.SetPen(wxPen(color0, 3));
		}
		else if (budget.percent > 50 && budget.percent <= 80) {
			dc.SetPen(wxPen(color50, 3));
		}
		else {
			dc.SetPen(wxPen(color100, 3));
		}

		if (width > progressWidth) {
			width = progressWidth;
		}

		dc.DrawLine(x, y, x + width, y);

		y = y + 5;

		dc.SetFont(budgetFont);
		
		if (budget.percent <= 100) {
			dc.SetTextForeground(wxColor(60, 60, 60));
		}
		else {
			dc.SetTextForeground(wxColor(185, 25, 0));
		}

		dc.DrawText(budget.currentAmount, wxPoint(x, y));

		size = dc.GetTextExtent(budget.amount);

		dc.SetFont(budgetFont);
		dc.SetTextForeground(wxColor(120, 120, 120));		
		
		dc.DrawText(budget.amount, wxPoint(x + progressWidth - size.GetWidth(), y));

		y = y + 40;
	}
}

void DashboardPanel::OnPaint(wxPaintEvent& event) {
	Draw();
}