#include <iostream>
using namespace std;

double CalculateFitness(double x)   { return x * x + 4; }

class Prim
{
    double gen;
public:

    Prim()                  { gen = 0; }

    Prim(double x)          { gen = x; }

    double GetGen()         { return gen; }

    void SetGen(double x)   { gen = x; }

    double SelfFitness()    { return CalculateFitness(gen); }

    Prim operator = (Prim& obj) { this->gen = obj.gen; return *this; }

    Prim(const Prim& obj) { this->gen = obj.gen; }

};

class Population
{
    Prim* mas;
    int size;
    double* MasFitness;
public:
    Population(int primNumber)
    {
        mas = new Prim[primNumber];
        size = primNumber;
        MasFitness = new double[primNumber];
    }

    ~Population()               
    { 
        delete[] mas;
        delete[] MasFitness;
    }

    Prim& operator [] (int i)   { return mas[i]; }

    int Size()                  { return size; }

    //Функция оценки популяции
    double CalculateFitnessPopulation()
    {
        double f = 0;
        for (int i = 0; i < size; i++)
        {
            MasFitness[i] = CalculateFitness(mas[i].GetGen());     //Целевая функция f = x * x + 4;
            f += MasFitness[i];
        }
        f = f / size;
        return f;
    }

    Population& operator = (Population& obj)
    {
        for (int i = 0; i < obj.size; i++)
            this->mas[i] = obj.mas[i];
        return *this;
    }

    Population(const Population& obj)
    {
        for (int i = 0; i < obj.size; i++)
            this->mas[i] = obj.mas[i];
    }
};

Prim& ClonePrim(Prim& individual)
{
    Prim newPrim;
    newPrim = individual;
    return newPrim;
}

//Функция турнирного отбора
Population& TournamentSelection(Population& population, int populationSize)
{
    Population *offspring = new Population(populationSize);
    //Population offspring(populationSize);
    for (int j = 0; j < populationSize; j++)
    {
        int i1 = 0, i2 = 0;
        while (i1 == i2)
        {
            i1 = rand() % populationSize;
            i2 = rand() % populationSize;
        }
        if (population[i1].SelfFitness() < population[i2].SelfFitness())
            (*offspring)[j] = ClonePrim(population[i1]);
            //(offspring)[j] = population[i1];
        else
            (* offspring)[j] = ClonePrim(population[i2]);
            //( offspring)[j] = population[i2];
    }
    return *offspring;
}

//Функция BLX-альфа кроссинговера
void BLXaCrossing(Prim& parent1, Prim& parent2)
{
    Prim child1, child2;
    double lambda = 0.55;
    child1.SetGen(lambda * parent1.GetGen() + (1 - lambda) * parent2.GetGen());     //ген первого потомка
    child2.SetGen(lambda * parent2.GetGen() + (1 - lambda) * parent1.GetGen());     //ген второго потомка
    parent1 = child1; parent2 = child2;                                             //родители заменяются на потомков
}

//Функция мутации индивида
void Mutation(Prim& individual)
{
    //Пусть величина мутации будет в диапазоне от [-0.25; 0.25]
    //Пусть вероятность мутации будет равна 5%
    if (rand() % 100 < 10)  //Вероятность мутации 10%
    {
        double newGen = (rand() % 26 - 25.0) / 100;     //Генерируем случайное число в указанном интервале
        newGen += individual.GetGen();                  //Прибавляем его к исходному гену
        individual.SetGen(newGen);                      //Теперь это ген i-ой особи
    }
}

int main()
{
    setlocale(LC_ALL, "ru");
    srand(time(0));
    int populationSize=2, generationCounter = 0, maxGeneration = 50, P_Crossing = 90;
    cout << "Введите количество особей в популяции:\n";
    //cin >> populationSize; cout << endl;                        //Ввод размера популяции

    //Генерация начальной популяции
    Population P(populationSize);
    for (int i = 0; i < populationSize; i++)
        P[i].SetGen((rand() % 201) - 100.0);                      //i-й особи присваивается значение в диапазоне [-100, 100]

    for (int i = 0; i < populationSize; i++)
        cout << P[i].GetGen() << endl;

    cout << "Func: " << P.CalculateFitnessPopulation();
    cout << "\n\n";

    double* maxFitnessValues = new double[50];  //значения наиболее приспособленной особи в i-ой популяции
    double* meanFitnessValue = new double[50];  //средние значения приспособленности особей в i-ой популяции

    //Population offspring(populationSize);
    //offspring = *TournamentSelection(P, populationSize);

    //for (int i = 0; i < populationSize; i++)
    //    cout << offspring[i].GetGen() << endl;

    //cout << "Func: " << offspring.CalculateEstimatePopulation();
    //cout << "\n\n";

    Population offspring(populationSize);

    //Собственно генетический алгоритм
    for (generationCounter = 0; generationCounter < 1/*maxGeneration*/; generationCounter++)
    {
        offspring = TournamentSelection(P, populationSize);
        //Цикл скрещивания
        for (int i = 0; i < populationSize/2; i++)
        {
            if (rand() % 100 < P_Crossing)                                      //Скрещивание происходит с вероятностью 90%
                BLXaCrossing(offspring[i], offspring[populationSize - i]);      //Скрещивается i с n-i особью, где n- размер популяции
        }

        //Цикл мутаций
        for (int i = 0; i < populationSize; i++)
        {
            Mutation(offspring[i]);
        }
        P = offspring;
    }

    cout << "SECOND:\n\n";

    for (int i = 0; i < populationSize; i++)
        cout << P[i].GetGen() << endl;

    cout << "Func: " << P.CalculateFitnessPopulation();
    cout << "\n\n";

    return 0;
}

