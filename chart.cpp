#include "chart.h"

Chart::Chart(QScatterSeries *series1, QString title, QString axisXLabel,QString axisYLabel, int rangeX, int rangeY, QString seriesName1,
             bool hasSeries2,  QLineSeries *series2, QString seriesName2,
             QWidget *parent)
    : QChartView(parent), m_countRotation(0), m_isPressed(false),
      m_rangeX(rangeX), m_rangeY(rangeY), m_series1(series1), m_hasSeries2(hasSeries2), m_series2(series2)
{
    if(m_hasSeries2)
    {
        m_chart = new QChart(); // utworzenie nowego obiektu wykresu
        m_series1->setName(seriesName1);
        m_series2->setName(seriesName2);

        m_chart->addSeries(series1);
        m_chart->addSeries(series2);


        m_pen1 = new QPen(QRgb(0xf40659));
        m_pen1->setWidth(1);
        m_series1->setPen(*m_pen1);
        m_pen2 = new QPen(QRgb(0x0033cc));
        m_pen2->setWidth(2);
        m_series2->setPen(*m_pen2);

    }else
    {
        m_chart = new QChart(); // utworzenie nowego obiektu wykresu
        m_series1->setName(seriesName1);
        m_chart->addSeries(m_series1);


        m_pen1 = new QPen(QRgb(0xf40659));
        m_pen1->setWidth(1);
        m_series1->setPen(*m_pen1);
    }


    // utworznie obiektu modyfikującego czcionkę
    QFont font;
    font.setPixelSize(18);
    m_chart->setTitleFont(font);    // przypisanie obiektu czcionki do tytułu wykresu
    m_chart->setTitleBrush(QBrush(Qt::gray)); // ustawienie kolory tytułu wykresu

    m_coord = new QGraphicsTextItem(m_chart); // utworzenie obiektu wyświetlającego koordynacje kursora na wykresie i przypisanie do wykresu
    m_coord->setZValue(11); // przeniesienie okna w górę hierarchii wyświetlanych elementów aby wykres nie przesłaniał

    m_chart->setTitle(title); // przypisanie łańcucha znaków do tytułu wykresu
    m_chart->createDefaultAxes(); // utworzenie domyślnych osi
    m_chart->setAnimationOptions(QChart::AllAnimations); // aktywacja animacji wszystkich elementów

    m_chart->axisX()->setRange(0,rangeX);   // ustawienie zakresu osi x
    m_chart->axisX()->setTitleText(axisXLabel); // ustawienie tytułu osi x

    m_chart->axisY()->setRange(0,rangeY);   // ustawienie zakresu osi y
    m_chart->axisY()->setTitleText(axisYLabel); // ustawienie tytułu osi y


    this->setChart(m_chart);    // przypisanie obiektu wykresu do sceny, czyli tej klasy
    this->setCursor(Qt::OpenHandCursor); // ustawienie kursora otwartej dłoni

}

Chart::~Chart()
{
    delete m_pen1;
    if(m_hasSeries2)
        delete m_pen2;
    delete m_coord; // usuwanie obiektu wyświetlającego koordynacje kursora
    delete m_chart; // usuwanie obiektu wykresu
}
// metoda odbierająca dane z sygnału najechania kursorem na wykres w postaci parametrów funkcji
void Chart::showWindowCoord(QPointF point, bool state)
{
    if(state) // sprawdzenie stanu położenia kursora, czy znajduje się nad wykresem
    {
        if(point.x() > m_rangeX*0.75) // sprawdzenie czy kursor znajduje się w 75% długości osi x jeżeli tak to...
            m_coord->setPos(m_chart->mapToPosition(point).x()-150, m_chart->mapToPosition(point).y()-40);// zmieniamy położenie okna z koordynacjami na lewą stronę
        else    //                                                                                          położenia kursora
            m_coord->setPos(m_chart->mapToPosition(point).x()+15, m_chart->mapToPosition(point).y()-40);// jeżeli nie to na prawą stronę
        m_coord->show(); // wyświetlenie okna
        this->setCursor(Qt::CrossCursor); // ustawienie kursora krzyża
    }else // jeżeli kursor nie znajduje się nad wykresem to ...
    {
        this->setCursor(Qt::OpenHandCursor); // ustawienie kursora otwartej dłoni
        m_coord->hide(); // ukrycie okna
    }
}

//  metoda obsługi zdarzeń pokrętła myszki
void Chart::wheelEvent(QWheelEvent *event)
{
    if(event->angleDelta().y() > 0)// jeżeli pokrętło jest kręcona od uzytkownika, to przyjmuje wartości dodatnie
    {
        chart()->zoomIn(); // przybliżenie wykresu
        m_countRotation++; // inkrementacja zmiennej przechowującej ilość wykonanych obrotów przybliżających
        m_pen1->setWidth(m_countRotation+2);
        m_series1->setPen(*m_pen1);
        if(m_hasSeries2)// jeżeli mamy dane z dwóch plików...
        {
            m_pen2->setWidth(m_countRotation+2);// zwiekszanie grubości linii podczas przybliżania
            m_series2->setPen(*m_pen2);
        }
    }
    else if(m_countRotation) // jeżeli zmienna przechowuje liczby dodatnie to...
    {
        chart()->zoomOut(); // oddal wykres
        m_countRotation--;  // dekrementacja liczb przybliżeń
        m_pen1->setWidth(m_countRotation+2);
        m_series1->setPen(*m_pen1);
        if(m_hasSeries2)// jeżeli mamy dane z dwóch plików...
        {
            m_pen2->setWidth(m_countRotation+2);// zmniejszenie grubości linii podczas oddalania
            m_series2->setPen(*m_pen2);
        }
    }else // jeżeli warość przybliżeń wynosi zero to...
    {
        chart()->axisX()->setRange(0, m_rangeX); // ustaw zakres osi x na domyślny
        chart()->axisY()->setRange(-m_rangeY, m_rangeY); // ustaw zakres osi y na domyślny
    }
    return QChartView::wheelEvent(event);
}
// metoda obsługi zdarzeń naciśnięcia klawisza myszki
void Chart::mousePressEvent(QMouseEvent *event)
{
    if(!m_isPressed) // jeżeli klawisz nie był wcześniej wciśnięty to...
    {
        m_isPressed = true;     // ustaw wartość informującą o wciśnięciu klawisza
        m_pos = event->pos();   // pobierz pozycję kursora
        this->setCursor(Qt::ClosedHandCursor); // ustawienie kursora zamkniętej dłoni
    }
    QChartView::mousePressEvent(event);
}
// metoda obsługi zdarzeń puszczenia klawisza myszki
void Chart::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_isPressed) // jeżeli klawisz myszki był wciśnięty to...
    {
        chart()->setAnimationOptions(QChart::SeriesAnimations); // ustaw animację serii wykresu
        m_isPressed = false; // ustaw wartość informującą o puszczeniu klawisza
        this->setCursor(Qt::OpenHandCursor); // ustawienie kursora otwartej dłoni
    }
    QChartView::mouseReleaseEvent(event);
}
// metoda obsługi zdarzeń przesuwania myszki
void Chart::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isPressed) // jeżeli klawisz myszki był wciśnięty to...
    {
        QPoint temp = event->pos();     // przechowaj aktualną pozycję kursora w zmiennej tymczasowej
        int deltaX = m_pos.x()-temp.x();        // obliczenie o ile przesunął się kursor od momentu przyciśnięcia klawisza myszki
        int deltaY = (m_pos.y()-temp.y())/-1;   // to samo dla osi y
        chart()->scroll(deltaX, deltaY);        // przesunięcie zakresu osi o wartości tego przesunięcia
        m_pos = temp;           // przypisanie aktualnej pozycji do zmiennej klasy
    }
    /*m_coord->setHtml("<div style='background-color: #ffffff; font-size: 15px;'>"+QString("x: %1\ny: %2")
                     .arg(m_chart->mapToValue(event->pos()).x())
                     .arg(m_chart->mapToValue(event->pos()).y())+"</div>"); */    // przypisanie koordynacji kursora do okna z informacją o położeniu
    QChartView::mouseMoveEvent(event);                                          // w postaci kodu html w celu przypisania koloru tła
}


