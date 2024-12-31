#include <QtWidgets>
#include <pugixml.hpp>

#include "channel.hpp"
#include "epg.hpp"
#include "epg_model.hpp"
#include "epg_view.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget window;

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("/home/casey/Downloads/xmltv.xml");

    Epg epg(doc);

    QStringList data;
    for (auto &[_, channel] : epg.channel_map) {
        data << QString::fromStdString(channel->num_string());
    }

    EpgView *table_view = new EpgView();
    table_view->setModel(new EpgModel(epg));

    // Layout to hold the list view
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(table_view);

    // Set the window properties
    window.setLayout(layout);

    window.resize(640, 480);
    window.show();
    window.setWindowTitle("recorder");

    return app.exec();
}
