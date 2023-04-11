#include<iostream>
class Base {//родительский класс
public:
	Base() {
		std::cout << "Base()" << std::endl;
	}
	Base(Base* obj) {
		std::cout << "Base(Base* obj)" << std::endl;
	}
	Base(Base& obj) {
		std::cout << "Base(Base& obj)" << std::endl;
	}
	virtual ~Base() {
		std::cout << "~Base()" << std::endl;
	}
	void doSomething1() {//невиртуальный метод 1
		std::cout << "Base::doSomething1" << std::endl;
		doSomething3();
	}
	void doSomething2() {//невиртуальный метод 2
		std::cout << "Base::doSomething1" << std::endl;
		doSomething4();
	}
	void doSomething3() {//невиртуальный метод 3
		std::cout << "Base::doSomething2 non virtual" << std::endl;
	}
	virtual void doSomething4() {//виртуальный метод 4
		std::cout << "Base::doSomething2 virtual" << std::endl;
	}
	virtual std::string classname() {//метод возвращения имени класс объекта
		return "Base";
	}
	virtual bool IsA(std::string classname) {//метод для предварительной проверки при приведении типа
		return classname == "Base";
	}
};
class Desk : public Base {//Дочерний класс
public:
	Desk() {
		std::cout << "Desk()" << std::endl;
	}
	Desk(Desk* obj) {
		std::cout << "Desk(Desk* obj)" << std::endl;
	}
	Desk(Desk& obj) {
		std::cout << "Desk(Desk& obj)" << std::endl;
	}
	virtual ~Desk() {
		std::cout << "~Desk()" << std::endl;
	}
	void doSomething3() {//невиртуальный метод 3
		std::cout << "Desk::doSomething2 non virtual" << std::endl;
	}
	void doSomething4() override {//виртуальный метод 4
		std::cout << "Desk::doSomething2 virtual" << std::endl;
	}
	std::string classname() override {
		return "Desk";
	}
	bool IsA(std::string classname) override {
		return classname == "Desk";
	}
};
void func() {
	std::cout << "func()" << std::endl;
}
void func(Base* obj) {
	std::cout << "func(Base* obj)" << std::endl;
	(static_cast<Desk*>(obj))->doSomething3();
}
void func(Base& obj) {
	std::cout << "func(Base& obj)" << std::endl;
	(static_cast<Desk*>(&obj))->doSomething3();
}
Base func1() {
	Base local;
	return local;
}
Base* func2() {
	Base local;
	return &local;
}
Base& func3() {
	Base local;
	return local;
}
Base func4() {
	Base* local = new Base();
	return *local;
}
Base* func5() {
	Base* local = new Base();
	return local;
}
Base& func6() {
	Base* local = new Base();
	return *local;
}
int main() {
	std::cout << "1)\n{\n";//жизненный цикл unique_ptr
	std::cout << "std::unique_ptr<Base> basa(new Base())" << std::endl;
	{
		std::unique_ptr <Base> basa(new Base());
	}
	std::cout << "}\n";
	std::cout << "2)\n{\n";//жизненный цикл shared_ptr
	std::cout << "std::shared_ptr<Base> basa(new Base())" << std::endl;
	{
		std::shared_ptr <Base> basa(new Base());
		{
			std::shared_ptr<Base> basa1 = basa;
			std::cout << basa << std::endl << basa1 << std::endl;
		}
	}
	std::cout << "}\n";
	std::cout << "3)\n{\n";//небезопасное приведение типов
	{
		std::cout << "(static_cast<Desk*>(stcast))->doSomething3()" << std::endl;
		Base* stcast = new Desk();
		if (stcast->IsA("Desk")) {
			(static_cast<Desk*>(stcast))->doSomething3();
		}
		delete stcast;
	}
	std::cout << "}\n";
	std::cout << "4)\n{\n";//безопасное приведение типов
	{
		std::cout << "(dynamic_cast<Desk*>(dcast))->doSomething3()" << std::endl;
		Base* dcast = new Desk();
		Desk* c = dynamic_cast<Desk*>(dcast);
		if (c != nullptr) {
			c->doSomething3();
		}
		delete dcast;
	}
	std::cout << "}\n";
	std::cout << "5)\n{\n";//вызов виртуальных и невиртуальных методов в дочернем объекте
	{
		Desk testDaughter;
		std::cout << "Result of Desk::doSomething1() and non virtual method2: " << std::endl;
		testDaughter.doSomething1();
		std::cout << "Result of Desk::doSomething1() and virtual method2: " << std::endl;
		testDaughter.doSomething2();
	}
	std::cout << "}\n";
	std::cout << "6)\n{\n";//передача объектов в качестве параметров
	{
		std::unique_ptr <Base> test(new Desk());
		func(*test);
		func(test.get());
	}
	std::cout << "}\n";
	std::cout << "7)\n{\n";//возврат созданных в функциях объектов
	{
		std::cout << "func1():\n";
		Base b1;
		b1 = func1();

		std::cout << "func2():\n";
		Base* b2;
		b2 = func2();

		std::cout << "func3():\n";
		Base& b3 = func3();

		std::cout << "func4():\n";
		Base b4;
		b4 = func4();

		std::cout << "func5():\n";
		Base* b5;
		b5 = func5();

		std::cout << "func6():\n";
		Base& b6 = func6();

	}
	std::cout << "}\n";
	return 0;
}