#include <iostream>
#include <vector>
#include <iomanip>


using namespace std;
class Time {

private:
	int hs;
	int min;
	int sec;
public:
	Time() {
		hs = 0;
		min = 0;
		sec = 0;
	}
	Time(int totsec) {
		hs = totsec / 3600;
		min = (totsec % 3600) / 60;
		sec = totsec % 60;
	}
	friend std::ostream& operator<<(std::ostream& str, const Time& time) {
		if (time.hs / 10 == 0) {
			str << "0" << time.hs << "h:";
		}
		else {
			str << time.hs << "h:";
		}
		if (time.min / 10 == 0) {
			str << "0" << time.min << "m:";
		}
		else {
			str << time.min << "m:";
		}
		if (time.sec / 10 == 0) {
			str << "0" << time.sec << "s";
		}
		else {
			str << time.sec << "s";
		}
		return str;

	}
	

	friend std::istream& operator>>(std::istream& str, Time& time) {
		str >> time.hs >>  time.min >> time.sec;
		return str;
	}
	Time operator+(const Time& _) const {
		int totsec = (hs + _.hs) * 3600 + (min + _.min) * 60 + (sec + _.sec);
		return Time(totsec);
	}
	Time operator-(const Time& _) const {
		int totsec = (hs - _.hs) * 3600 + (min - _.min) * 60 + (sec - _.sec);
		return Time(totsec);
	}
	Time operator*(const Time& _) const {
		int totsec = (hs * _.hs) * 3600 + (min * _.min) * 60 + (sec * _.sec);
		return Time(totsec);
	}
	
	operator int() const {
		return hs * 3600 + min * 60 + sec;
	}


};


/// <summary>
/// /////////////////////////////////////////////////////////
/// </summary>
class Item {
private:
	string name;
	double unp;
	char vat;
	int am_sold;
public:
	Item(string name_, double unp_, char vat_, int am_sold_)
	{
		name = name_;
		unp = unp_;
		vat = vat_;
		am_sold = am_sold_;
	}
	double tot_pr() const
	{
		double pr_vat=0.0;
		if (vat == 'A')
			pr_vat = 0.23;
		if (vat == 'B')
			pr_vat = 0.08;
		if (vat == 'C')
			pr_vat = 0.0;

		return unp * am_sold * (1 + pr_vat);
	}
	friend std::ostream& operator<<(std::ostream& str, const Item& item)
	{
		str << setw(12) << item.name << "| ";
		str << setw(6) << item.unp << " " << item.vat << " | ";
		str << setw(3) << item.am_sold << " | ";
		str << setw(5) << item.unp * item.am_sold << " | ";
		str << setw(6) << setprecision(3)<<item.tot_pr();


		return str;

	}
	friend class Invoice;





};
/// <summary>
/// /////////////////////////////////////////
/// </summary>

class Invoice {
private:
	long long nip_s;
	long long nip_b;
	vector<Item> item;
public:
	Invoice(long long nip_s_, long long nip_b_) {
		nip_s = nip_s_;
		nip_b = nip_b_;
	}
	void add_item(const Item& item_) {
		item.emplace_back(item_);
	}
	double total_net() const {
		double total = 0.0;
		for (const Item& item_ : item) {
			total += item_.unp * item_.am_sold;
		}
		return total;
	}
	double total() const {
		double total = 0.0;
		for (const Item& item_ : item) {
			total += item_.tot_pr() ;
		}
		return total;
	}
	friend std::ostream& operator<<(std::ostream& str, const Invoice& inv) {
		str << "------------------VAT invoice------------------\n";
		str << "===============================================\n";
		str << "Seller: " << inv.nip_s << "       Buyer: " << inv.nip_b;
		str << "\n\n                c.j. VAT   il.    net   total" << endl;;
		int k = 0;
		for (const Item& item_ : inv.item) {
			k++;;
			str << k << "." << item_ << endl;
		}
		str << "\n------------------------------------ TOTAL ----\n";
		str << "                                  " << inv.total_net() << " | " << inv.total();
		return str;
	}
		


};

int main()
{

	Time t1(200);
	cout << t1 << endl;
	Time t2(36721);
	//cin >> t2; doesnt work
	cout << t2 << " cin >> t2; doesnt work" << endl;
	Time t3 = t2 - t1;
	cout << t3 << endl;
	int t3s = t3;
	cout << t3s;
	cout << "\n\n\n\n\n\n";
	Invoice inv(7770003699, 0123456700);
	inv.add_item(Item("M3 screw", 0.37, 'A', 100));
	inv.add_item(Item("2 mm drill", 2.54, 'B', 2));
	cout << inv << endl;
}

