#include "Widgets/VectorEditor.hpp"
#include <Gui/Widgets/ControlPanel.hpp>

#include <Gui/Widgets/CheckingSpinBox.hpp>
#include <Gui/Widgets/MatrixEditor.hpp>
#include <Gui/Widgets/PowerSlider.hpp>
#include <Gui/Widgets/VectorEditor.hpp>

#include <QLabel>
#include <QVBoxLayout>

#include <QColorDialog>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>

namespace Ra::Gui::Widgets {

ControlPanel::ControlPanel( const std::string& name, bool hline, QWidget* parent ) :
    QFrame( parent ) {
    setObjectName( name.c_str() );

    m_currentLayout = new QVBoxLayout( this );
    setLayout( m_currentLayout );
    if ( hline ) {
        auto panelName = new QLabel( this );
        panelName->setFrameStyle( QFrame::HLine );
        m_currentLayout->addWidget( panelName );
    }
    // addSeparator();
    setVisible( false );
    m_layouts.push( m_currentLayout );
}

// Method to populate the panel
void ControlPanel::addOption( const std::string& name,
                              std::function<void( bool )> callback,
                              bool set,
                              const std::string& tooltip ) {
    auto button = new QRadioButton( name.c_str(), this );
    button->setLayoutDirection( Qt::RightToLeft );
    button->setAutoExclusive( false );
    button->setChecked( set );
    if ( !tooltip.empty() ) {
        button->setToolTip(
            QString( "<qt>%1</qt>" ).arg( QString( tooltip.c_str() ).toHtmlEscaped() ) );
    }
    m_currentLayout->addWidget( button );
    connect( button, &QRadioButton::toggled, std::move( callback ) );
}

void ControlPanel::addLabel( const std::string& text ) {
    auto label = new QLabel( text.c_str(), nullptr );
    m_currentLayout->addWidget( label );
}

void ControlPanel::addButton( const std::string& name,
                              std::function<void()> callback,
                              const std::string& tooltip ) {
    auto button = new QPushButton( name.c_str(), nullptr );
    if ( !tooltip.empty() ) {
        button->setToolTip(
            QString( "<qt>%1</qt>" ).arg( QString( tooltip.c_str() ).toHtmlEscaped() ) );
    }
    m_currentLayout->addWidget( button );
    connect( button, &QPushButton::clicked, std::move( callback ) );
}

void ControlPanel::addScalarInput( const std::string& name,
                                   std::function<void( Scalar )> callback,
                                   Scalar initial,
                                   Scalar min,
                                   Scalar max,
                                   int dec,
                                   const std::string& tooltip ) {
    auto inputLayout = new QHBoxLayout();

    auto inputLabel = new QLabel( tr( name.c_str() ), this );
    auto inputField = new QDoubleSpinBox( this );

    if ( !tooltip.empty() ) {
        inputLabel->setToolTip(
            QString( "<qt>%1</qt>" ).arg( QString( tooltip.c_str() ).toHtmlEscaped() ) );
    }
    inputField->setMinimum( min );
    inputField->setMaximum( max );
    inputField->setDecimals( dec );
    inputField->setValue( initial );

    inputLayout->addWidget( inputLabel );
    inputLayout->addWidget( inputField );
    connect(
        inputField, QOverload<double>::of( &QDoubleSpinBox::valueChanged ), std::move( callback ) );
    m_currentLayout->addLayout( inputLayout );
}

void ControlPanel::addCheckingScalarInput( const std::string& name,
                                           std::function<void( Scalar )> callback,
                                           Scalar initial,
                                           std::function<bool( double )> predicate,
                                           int dec,
                                           const std::string& tooltip ) {
    auto inputLayout = new QHBoxLayout();

    auto inputLabel = new QLabel( tr( name.c_str() ), this );
    auto inputField = new CheckingSpinBox( this );

    if ( !tooltip.empty() ) {
        inputLabel->setToolTip(
            QString( "<qt>%1</qt>" ).arg( QString( tooltip.c_str() ).toHtmlEscaped() ) );
    }
    inputField->setDecimals( dec );
    inputField->setValue( initial );
    inputField->setPredicate( predicate );

    inputLayout->addWidget( inputLabel );
    inputLayout->addWidget( inputField );
    connect( inputField, &CheckingSpinBox::valueChanged, std::move( callback ) );
    m_currentLayout->addLayout( inputLayout );
}

void ControlPanel::addSliderInput( const std::string& name,
                                   std::function<void( int )> callback,
                                   int initial,
                                   int min,
                                   int max,
                                   const std::string& tooltip ) {
    auto inputLayout = new QHBoxLayout();
    auto inputLabel  = new QLabel( tr( name.c_str() ), this );
    auto inputField  = new QSlider( Qt::Horizontal, this );
    if ( !tooltip.empty() ) {
        inputLabel->setToolTip(
            QString( "<qt>%1</qt>" ).arg( QString( tooltip.c_str() ).toHtmlEscaped() ) );
    }
    inputField->setRange( min, max );
    inputField->setValue( initial );
    inputLayout->addWidget( inputLabel );
    inputLayout->addWidget( inputField );
    connect( inputField, &QSlider::valueChanged, std::move( callback ) );
    m_currentLayout->addLayout( inputLayout );
}

void ControlPanel::addPowerSliderInput( const std::string& name,
                                        std::function<void( double )> callback,
                                        double initial,
                                        double min,
                                        double max,
                                        const std::string& tooltip ) {
    auto inputLayout = new QHBoxLayout();
    auto inputLabel  = new QLabel( tr( name.c_str() ), this );
    auto inputField  = new PowerSlider();
    if ( !tooltip.empty() ) {
        inputLabel->setToolTip(
            QString( "<qt>%1</qt>" ).arg( QString( tooltip.c_str() ).toHtmlEscaped() ) );
    }
    inputField->setObjectName( name.c_str() );
    inputField->setRange( min, max );
    inputField->setValue( initial );
    inputField->setSingleStep( 0.01 );
    inputLayout->addWidget( inputLabel );
    inputLayout->addWidget( inputField );
    connect( inputField, &PowerSlider::valueChanged, std::move( callback ) );
    m_currentLayout->addLayout( inputLayout );
}

void ControlPanel::addVectorInput( const std::string& name,
                                   std::function<void( const std::vector<double>& )> callback,
                                   const std::vector<double>& initial,
                                   int dec,
                                   const std::string& tooltip ) {
    auto inputLayout = new QHBoxLayout();

    auto inputLabel = new QLabel( tr( name.c_str() ), this );
    auto inputField = new VectorEditor( initial, dec, this );

    if ( !tooltip.empty() ) {
        inputLabel->setToolTip(
            QString( "<qt>%1</qt>" ).arg( QString( tooltip.c_str() ).toHtmlEscaped() ) );
    }
    inputLayout->addWidget( inputLabel );
    inputLayout->addWidget( inputField );
    connect( inputField, &VectorEditor::valueChanged, std::move( callback ) );
    m_currentLayout->addLayout( inputLayout );
}

void ControlPanel::addMatrixInput( const std::string& name,
                                   std::function<void( const Ra::Core::MatrixN& )> callback,
                                   const Ra::Core::MatrixN& initial,
                                   int dec,
                                   const std::string& tooltip ) {
    auto inputLayout = new QHBoxLayout();

    auto inputLabel = new QLabel( tr( name.c_str() ), this );
    auto inputField = new MatrixEditor( initial, dec, this );

    if ( !tooltip.empty() ) {
        inputLabel->setToolTip(
            QString( "<qt>%1</qt>" ).arg( QString( tooltip.c_str() ).toHtmlEscaped() ) );
    }
    inputLayout->addWidget( inputLabel );
    inputLayout->addWidget( inputField );
    connect( inputField, &MatrixEditor::valueChanged, std::move( callback ) );
    m_currentLayout->addLayout( inputLayout );
}

void ControlPanel::addColorInput(
    const std::string& name,
    const std::function<void( const Ra::Core::Utils::Color& clr )>& callback,
    Ra::Core::Utils::Color color,
    const std::string& tooltip ) {
    auto button = new QPushButton( name.c_str(), this );
    if ( !tooltip.empty() ) {
        button->setToolTip(
            QString( "<qt>%1</qt>" ).arg( QString( tooltip.c_str() ).toHtmlEscaped() ) );
    }
    auto rgbValue = color.rgb();
    auto clrBttn  = QColor::fromRgb(
        int( rgbValue[0] * 255 ), int( rgbValue[1] * 255 ), int( rgbValue[2] * 255 ) );
    {
        QString qss = QString( "background-color: %1" ).arg( clrBttn.name() );
        button->setStyleSheet( qss );
    }
    auto clrDlg = [callback, clrBttn, button, name]() mutable {
        QColor c = QColorDialog::getColor( clrBttn, button, name.c_str() );
        if ( c.isValid() ) {
            // update the background coolor of the viewer
            auto bgk = Ra::Core::Utils::Color::sRGBToLinearRGB( Ra::Core::Utils::Color(
                Scalar( c.redF() ), Scalar( c.greenF() ), Scalar( c.blueF() ), Scalar( 0 ) ) );
            callback( bgk );
            clrBttn     = c;
            QString qss = QString( "background-color: %1" ).arg( c.name() );
            button->setStyleSheet( qss );
        }
        return c;
    };
    connect( button, &QPushButton::clicked, clrDlg );
    m_currentLayout->addWidget( button );
}

void ControlPanel::addFileInput( const std::string& name,
                                 std::function<void( std::string )> callback,
                                 const std::string& rootDirectory,
                                 const std::string& filters,
                                 const std::string& tooltip ) {
    auto button = new QPushButton( name.c_str(), this );
    if ( !tooltip.empty() ) {
        button->setToolTip(
            QString( "<qt>%1</qt>" ).arg( QString( tooltip.c_str() ).toHtmlEscaped() ) );
    }
    auto fileDialog = [this, callback, rootDirectory, filters]() {
        auto fltrs    = QString::fromStdString( filters );
        auto pathList = QFileDialog::getOpenFileNames(
            this, "Open files", QString::fromStdString( rootDirectory ), fltrs );
        if ( !pathList.empty() ) {
            std::string fileList;
            for ( const auto& file : pathList ) {
                fileList += file.toStdString() + ";";
            }
            fileList.erase( fileList.size() - 1 );
            callback( fileList );
        }
        else { callback( "" ); }
    };
    connect( button, &QPushButton::clicked, fileDialog );
    m_currentLayout->addWidget( button );
}

void ControlPanel::addFileOuput( const std::string& name,
                                 std::function<void( std::string )> callback,
                                 const std::string& rootDirectory,
                                 const std::string& filters,
                                 const std::string& tooltip ) {
    auto button = new QPushButton( name.c_str(), this );
    if ( !tooltip.empty() ) {
        button->setToolTip(
            QString( "<qt>%1</qt>" ).arg( QString( tooltip.c_str() ).toHtmlEscaped() ) );
    }
    auto fileDialog = [this, callback, rootDirectory, filters]() {
        auto fltrs    = QString::fromStdString( filters );
        auto filename = QFileDialog::getSaveFileName(
            this, "Save file", QString::fromStdString( rootDirectory ), fltrs );
        if ( !filename.isEmpty() ) { callback( filename.toStdString() ); }
    };
    connect( button, &QPushButton::clicked, fileDialog );
    m_currentLayout->addWidget( button );
}

void ControlPanel::beginLayout( QBoxLayout::Direction dir ) {
    m_layouts.push( m_currentLayout );
    m_currentLayout = new QBoxLayout( dir );
}

void ControlPanel::endLayout( bool separator ) {
    m_layouts.top()->addLayout( m_currentLayout );
    m_currentLayout = m_layouts.top();
    if ( separator ) { addSeparator(); }
    m_layouts.pop();
}

void ControlPanel::addSeparator() {
    QFrame* line = new QFrame();
    switch ( m_currentLayout->direction() ) {
    case QBoxLayout::LeftToRight:
    case QBoxLayout::RightToLeft:
        line->setFrameShape( QFrame::VLine );
        break;
    case QBoxLayout::TopToBottom:
    case QBoxLayout::BottomToTop:
        line->setFrameShape( QFrame::HLine );
        break;
    default:
        line->setFrameShape( QFrame::HLine );
    }
    line->setFrameShadow( QFrame::Sunken );
    m_currentLayout->addWidget( line );
}

void ControlPanel::addComboBox( const std::string& name,
                                std::function<void( int )> callback,
                                int initial,
                                const std::vector<std::string>& items,
                                const std::string& tooltip ) {
    auto inputLayout = new QHBoxLayout();
    auto inputLabel  = new QLabel( tr( name.c_str() ), this );
    auto inputField  = new QComboBox( this );
    for ( auto v : items ) {
        inputField->addItem( QString::fromStdString( v ) );
    }
    if ( !tooltip.empty() ) {
        inputLabel->setToolTip(
            QString( "<qt>%1</qt>" ).arg( QString( tooltip.c_str() ).toHtmlEscaped() ) );
    }
    inputField->setCurrentIndex( initial );
    inputLayout->addWidget( inputLabel );
    inputLayout->addWidget( inputField );
    connect(
        inputField, QOverload<int>::of( &QComboBox::currentIndexChanged ), std::move( callback ) );
    m_currentLayout->addLayout( inputLayout );
}

void ControlPanel::addStretch( int stretch ) {
    m_currentLayout->addStretch( stretch );
}

void ControlPanel::addWidget( QWidget* newWidget ) {
    m_currentLayout->addWidget( newWidget );
}

} // namespace Ra::Gui::Widgets
