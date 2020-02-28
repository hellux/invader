// SPDX-License-Identifier: GPL-3.0-only

#include <QCloseEvent>
#include <QMessageBox>
#include <QMenuBar>
#include <QScrollArea>
#include <QLabel>
#include <QApplication>
#include <QScreen>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <filesystem>
#include <invader/file/file.hpp>
#include <invader/tag/parser/parser.hpp>
#include "tag_file.hpp"
#include "tag_tree_window.hpp"
#include "tag_editor_textbox_widget.hpp"

namespace Invader::EditQt {
    TagEditorWindow::TagEditorWindow(QWidget *parent, TagTreeWindow *parent_window, const TagFile &tag_file) : QMainWindow(parent), parent_window(parent_window), file(tag_file) {
        this->make_dirty(false);

        auto open_file = File::open_file(tag_file.full_path.string().c_str());
        if(!open_file.has_value()) {
            char formatted_error[1024];
            std::snprintf(formatted_error, sizeof(formatted_error), "Failed to open %s. Make sure it exists and you have permission to open it.", tag_file.full_path.string().c_str());
            QMessageBox(QMessageBox::Icon::Critical, "Error", formatted_error, QMessageBox::Ok, this).exec();
            this->close();
            return;
        }
        try {
            this->parser_data = Parser::ParserStruct::parse_hek_tag_file(open_file->data(), open_file->size(), false).release();
        }
        catch(std::exception &e) {
            char formatted_error[1024];
            std::snprintf(formatted_error, sizeof(formatted_error), "Failed to open %s due to an exception error:\n\n%s\n\nThe tag may be corrupt.", tag_file.full_path.string().c_str(), e.what());
            QMessageBox(QMessageBox::Icon::Critical, "Error", formatted_error, QMessageBox::Ok, this).exec();
            this->close();
            return;
        }

        // Make and set our menu bar
        QMenuBar *bar = new QMenuBar(this);
        this->setMenuBar(bar);

        // File menu
        auto *file_menu = bar->addMenu("File");

        auto *save = file_menu->addAction("Save");
        save->setIcon(QIcon::fromTheme(QStringLiteral("document-save")));
        save->setShortcut(QKeySequence::Save);
        connect(save, &QAction::triggered, this, &TagEditorWindow::perform_save);

        auto *save_as = file_menu->addAction("Save as...");
        save_as->setIcon(QIcon::fromTheme(QStringLiteral("document-save-as")));
        save_as->setShortcut(QKeySequence::SaveAs);
        connect(save_as, &QAction::triggered, this, &TagEditorWindow::perform_save_as);

        file_menu->addSeparator();

        auto *refactor = file_menu->addAction("Refactor...");
        refactor->setIcon(QIcon::fromTheme(QStringLiteral("edit-rename")));
        connect(refactor, &QAction::triggered, this, &TagEditorWindow::perform_refactor);

        file_menu->addSeparator();

        auto *close = file_menu->addAction("Close");
        close->setShortcut(QKeySequence::Close);
        close->setIcon(QIcon::fromTheme(QStringLiteral("document-close")));
        connect(close, &QAction::triggered, this, &TagEditorWindow::close);

        // Set up the scroll area
        auto *scroll_view = new QScrollArea(this);
        this->setCentralWidget(scroll_view);
        auto *vbox_layout = new QVBoxLayout(scroll_view);
        auto *full_widget = new QWidget(this);

        // TEST: Add widgets
        auto values = this->parser_data->get_values();
        for(auto &value : values) {
            auto add_simple_bar = [&vbox_layout, &full_widget, &value](TagEditorTextboxWidget::TextboxSize size, const char *suffix = nullptr) {
                std::vector<std::string> suffixes;

                auto value_count = value.get_value_count();
                auto split = value_count / 2;

                for(std::size_t i = 0; i < value_count; i++) {
                    if(i == split && value.is_bounds()) {
                        suffixes.emplace_back("-");
                    }
                    else {
                        suffixes.emplace_back();
                    }
                }

                if(suffix) {
                    suffixes.emplace_back(suffix);
                }

                TagEditorTextboxWidget *textbox = new TagEditorTextboxWidget(full_widget, value.get_name(), size, value_count, suffixes);
                std::vector<Parser::ParserStructValue::Number> values(value_count);
                value.get_values(values.data());

                switch(value.get_number_format()) {
                    case Parser::ParserStructValue::NumberFormat::NUMBER_FORMAT_FLOAT:
                        for(std::size_t v = 0; v < value_count; v++) {
                            double multiplier = value.get_type() == Parser::ParserStructValue::ValueType::VALUE_TYPE_ANGLE ? 180.0 / HALO_PI : 1;

                            textbox->set_float(std::get<double>(values[v]) * multiplier, v);
                        }
                        break;
                    case Parser::ParserStructValue::NumberFormat::NUMBER_FORMAT_INT:
                        for(std::size_t v = 0; v < value_count; v++) {
                            textbox->set_int(std::get<std::int64_t>(values[v]), v);
                        }
                        break;
                    default:
                        break;
                }

                vbox_layout->addWidget(textbox);
            };

            switch(value.get_type()) {
                case Parser::ParserStructValue::VALUE_TYPE_INT8:
                    add_simple_bar(TagEditorTextboxWidget::TextboxSize::SMALL);
                    break;
                case Parser::ParserStructValue::VALUE_TYPE_UINT8:
                    add_simple_bar(TagEditorTextboxWidget::TextboxSize::SMALL);
                    break;
                case Parser::ParserStructValue::VALUE_TYPE_INT16:
                    add_simple_bar(TagEditorTextboxWidget::TextboxSize::SMALL);
                    break;
                case Parser::ParserStructValue::VALUE_TYPE_UINT16:
                    add_simple_bar(TagEditorTextboxWidget::TextboxSize::SMALL);
                    break;
                case Parser::ParserStructValue::VALUE_TYPE_INDEX:
                    add_simple_bar(TagEditorTextboxWidget::TextboxSize::SMALL);
                    break;
                case Parser::ParserStructValue::VALUE_TYPE_INT32:
                    add_simple_bar(TagEditorTextboxWidget::TextboxSize::MEDIUM);
                    break;
                case Parser::ParserStructValue::VALUE_TYPE_UINT32:
                    add_simple_bar(TagEditorTextboxWidget::TextboxSize::MEDIUM);
                    break;
                case Parser::ParserStructValue::VALUE_TYPE_FLOAT:
                    add_simple_bar(TagEditorTextboxWidget::TextboxSize::MEDIUM);
                    break;
                case Parser::ParserStructValue::VALUE_TYPE_FRACTION:
                    add_simple_bar(TagEditorTextboxWidget::TextboxSize::MEDIUM);
                    break;
                case Parser::ParserStructValue::VALUE_TYPE_ANGLE:
                    add_simple_bar(TagEditorTextboxWidget::TextboxSize::MEDIUM, "degrees");
                    break;

                case Parser::ParserStructValue::VALUE_TYPE_COLORARGBINT:
                case Parser::ParserStructValue::VALUE_TYPE_POINT2DINT:
                case Parser::ParserStructValue::VALUE_TYPE_RECTANGLE2D:
                case Parser::ParserStructValue::VALUE_TYPE_COLORARGB:
                case Parser::ParserStructValue::VALUE_TYPE_COLORRGB:
                case Parser::ParserStructValue::VALUE_TYPE_VECTOR2D:
                case Parser::ParserStructValue::VALUE_TYPE_VECTOR3D:
                case Parser::ParserStructValue::VALUE_TYPE_EULER2D:
                case Parser::ParserStructValue::VALUE_TYPE_EULER3D:
                case Parser::ParserStructValue::VALUE_TYPE_PLANE2D:
                case Parser::ParserStructValue::VALUE_TYPE_PLANE3D:
                case Parser::ParserStructValue::VALUE_TYPE_POINT2D:
                case Parser::ParserStructValue::VALUE_TYPE_POINT3D:
                case Parser::ParserStructValue::VALUE_TYPE_QUATERNION:
                case Parser::ParserStructValue::VALUE_TYPE_MATRIX:
                    break;

                // Other stuff
                case Parser::ParserStructValue::VALUE_TYPE_DATA:
                case Parser::ParserStructValue::VALUE_TYPE_REFLEXIVE:
                case Parser::ParserStructValue::VALUE_TYPE_DEPENDENCY:
                case Parser::ParserStructValue::VALUE_TYPE_TAGSTRING:
                case Parser::ParserStructValue::VALUE_TYPE_TAGDATAOFFSET:
                case Parser::ParserStructValue::VALUE_TYPE_ENUM:
                case Parser::ParserStructValue::VALUE_TYPE_BITMASK:
                    break;
            }
        }

        /*std::vector<std::string> xyz;
        xyz.emplace_back("x");
        xyz.emplace_back("y");
        xyz.emplace_back("z");
        for(std::size_t i = 0; i < 16; i++) {
            char widget_name[256];
            std::snprintf(widget_name, sizeof(widget_name), "Test #%zu", i);
            TagEditorTextboxWidget *textbox = new TagEditorTextboxWidget(full_widget, widget_name, static_cast<TagEditorTextboxWidget::TextboxSize>(i % (TagEditorTextboxWidget::TextboxSize::LARGE + 1)), 3, xyz);
            vbox_layout->addWidget(textbox);
        }*/

        // Add a spacer so it doesn't try to evenly space everything if we're too big
        auto *spacer = new QSpacerItem(0 ,0);
        vbox_layout->addSpacerItem(spacer);
        vbox_layout->setSpacing(2);
        full_widget->setLayout(vbox_layout);
        scroll_view->setWidget(full_widget);

        // Lock the scroll view and window to a set width
        int max_width = full_widget->width() + qApp->style()->pixelMetric(QStyle::PM_ScrollBarExtent) + 50;
        scroll_view->setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Preferred);
        scroll_view->setMinimumWidth(max_width);
        scroll_view->setMaximumWidth(max_width);
        scroll_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        this->setMaximumWidth(scroll_view->width());

        // Center this
        this->setGeometry(
            QStyle::alignedRect(
                Qt::LeftToRight,
                Qt::AlignCenter,
                QSize(this->maximumWidth(), 600),
                QGuiApplication::primaryScreen()->geometry()
            )
        );
    }

    void TagEditorWindow::closeEvent(QCloseEvent *event) {
        bool accept;
        if(dirty) {
            char message_entire_text[512];
            std::snprintf(message_entire_text, sizeof(message_entire_text), "This file \"%s\" has been modified.\nDo you want to save your changes?", this->file.full_path.string().c_str());
            QMessageBox are_you_sure(QMessageBox::Icon::Question, "Unsaved changes", message_entire_text, QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, this);
            switch(are_you_sure.exec()) {
                case QMessageBox::Accepted:
                    accept = this->perform_save();
                    break;
                case QMessageBox::Cancel:
                    accept = false;
                    break;
                case QMessageBox::Discard:
                    accept = true;
                    break;
                default:
                    std::terminate();
            }
        }
        else {
            accept = true;
        }

        event->setAccepted(accept);
    }

    bool TagEditorWindow::perform_save() {
        std::fprintf(stderr, "TODO: perform_save()\n");
        return false;
    }

    bool TagEditorWindow::perform_save_as() {
        std::fprintf(stderr, "TODO: perform_save_as()\n");
        return false;
    }

    bool TagEditorWindow::perform_refactor() {
        std::fprintf(stderr, "TODO: perform_refactor()\n");
        return false;
    }

    void TagEditorWindow::make_dirty(bool dirty) {
        this->dirty = dirty;
        char title_bar[512];
        const char *asterisk = dirty ? " *" : "";
        std::snprintf(title_bar, sizeof(title_bar), "%s%s", this->file.tag_path.c_str(), asterisk);
        this->setWindowTitle(title_bar);
    }

    const TagFile &TagEditorWindow::get_file() const noexcept {
        return this->file;
    }

    TagEditorWindow::~TagEditorWindow() {
        delete this->parser_data;
    }
}
