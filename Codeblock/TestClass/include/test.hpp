#ifndef TEST_H
#define TEST_H


class test
{
    const int c;
    public:
        //test();
        test(int);
        virtual ~test();
        unsigned int Getm_Counter() { return m_Counter; }
        const int Gettest() { return m_test; }

    protected:
    private:
        const unsigned int m_Counter{0};
        const int m_test{0};
};

#endif // TEST_H
