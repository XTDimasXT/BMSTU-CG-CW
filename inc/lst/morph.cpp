void MyOpenGLWidget::Morph(FigureFacade &figure, FigureFacade &image)
{
    if (image.points_.array_points_.empty()) 
    {
        copy_image(figure, image);
        return;
    }

    int transformTime = 100;
    int delayMilliseconds = 100;
    QElapsedTimer tm;
    tm.start();
    size_t count = std::min(figure.points_.array_points_.size(), image.points_.array_points_.size());

    if (count == image.points_.array_points_.size())
    {
        for (size_t i = count; i < figure.points_.array_points_.size(); ++i) {
        image.points_.array_points_.push_back(image.points_.array_points_[i -
        count]);
        }
        count = figure.points_.array_points_.size();
    } 
    else 
    {
        size_t imageSize = image.points_.array_points_.size();
        for (size_t i = count; i < imageSize; ++i)
            image.points_.array_points_.erase(image.points_.array_points_.begin() + i);
    }
    std::vector<Point> steps(count);

    for (size_t i = 0; i < count; ++i) 
    {
        steps[i].x_ = (figure.points_.array_points_[i].x_ - image.points_.array_points_[i].x_) / transformTime;
        steps[i].y_ = (figure.points_.array_points_[i].y_ - image.points_.array_points_[i].y_) / transformTime;
        steps[i].z_ = (figure.points_.array_points_[i].z_ - image.points_.array_points_[i].z_) / transformTime;
    }
    image.edges_ = figure.edges_;
    QTimer* timer = new QTimer(this);
    QEventLoop eventLoop;
    connect(timer, &QTimer::timeout, this, [this, figure, &image, steps, transformTime, timer, &eventLoop]() mutable 
    {
        MorphNextStep(image, steps);
        --transformTime;
        if (transformTime < 0) 
        {
            copy_image(figure, image);
            timer->stop();
            timer->deleteLater();
            eventLoop.quit();
        }
    });

    MORPH = true;
    timer->start(delayMilliseconds);
    eventLoop.exec();
    QThread::msleep(1000);
    MORPH = false;
}

void MyOpenGLWidget::MorphNextStep(FigureFacade &image, std::vector<Point> &steps) 
{
    for (size_t i = 0; i < steps.size(); ++i) 
    {
        image.points_.array_points_[i].x_ += steps[i].x_;
        image.points_.array_points_[i].y_ += steps[i].y_;
        image.points_.array_points_[i].z_ += steps[i].z_;
    }

    update();
}
