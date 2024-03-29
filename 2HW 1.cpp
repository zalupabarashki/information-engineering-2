﻿#include <iostream>
#include <vector>
#include <numeric>


using namespace std;

class Student {

private:
    string name;
    string surname;
    vector<float> grades;
    string album_number;
public:
   
    void add_name(string s_name)
    {
        name =  s_name;
    }
    void add_surname(string s_surname)
    {
          surname = s_surname;
    }
    bool add_id(string  s_album_number) {
        if (s_album_number.size() >= 5 && s_album_number.size() <= 6) {

            album_number = s_album_number;
            return true;
        }
        return false;
    }
    bool add_grade(float grade) {
        if (grade >= 2.0 && grade <= 5.0) {
           
            grades.push_back(grade);
            return true;
        }
        return false;
    }
    void disp_sum()
    {
        cout << "Student: " << name << " " << surname << endl;
        cout << "Album Number: " << album_number << endl;
        cout << "Grades: ";
        for (const auto& grade : grades) {
            cout << grade << " ";
        }
        cout <<"\n";
    }

    float calculate_mean_grade()
    {
        float sum = accumulate(grades.begin(), grades.end(), 0.0f);
        return sum / grades.size();
    }
    void passed_semester()
    {
        int k = 0;
        for (const auto grade : grades) {
            if (grade == 2.0)
                k++;
        }if (k <= 1) {
            cout << "Student passed the semester\n";
        }
        else {
            cout << "Student didn't pass the semester\n";
        }
   }
};

class Complex {
private:
    double rl;
    double im;

public:
   

    Complex(double rl_ = 0.0, double im_ = 0.0) {
        rl = rl_;
        im = im_;
    }
    


    void print()
    {
        cout << rl;
        if (im >= 0) {
            cout << "+";
        }

        cout << im << "i" << endl;
    }

   
    double get_rl()  const
    { 
        return rl; 
    }
    void set_rl(double rl_) 
    {
        rl = rl_;
    }
    double get_im() const
    { 
        return im;
    }
    void set_im(double im_)
    { 
        im = im_;
    }
    Complex add(const Complex& i) const {
        double result_rl = rl + i.get_rl();
        double result_im = im + i.get_im();
        return Complex(result_rl, result_im);
    }
};


int main()
{
    Student s;

    s.add_name("Name");
    s.add_surname("Surname");
    s.add_id("12345");
    s.add_grade(4.5);
    s.add_grade(3.5);
    s.add_grade(2.0);
    s.add_grade(3.5);
    s.add_grade(2.0);
    s.disp_sum();
    cout << "Mean Grade: " << s.calculate_mean_grade() << endl;
    s.passed_semester();

    Complex a(1.0, -2.0); 
    Complex b(3.14); 

    b.set_im(-5);

    Complex c = a.add(b);

    c.print(); 
    return 0;
}