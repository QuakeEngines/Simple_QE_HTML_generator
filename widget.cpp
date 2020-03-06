#include "widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{

    setWindowTitle("Simple QuakeEngine HTML generator");
    resize(700, 320);

    QPushButton *generateButton = new QPushButton("Generate");
    QPushButton *aboutButton = new QPushButton("About");

    resultText = new QPlainTextEdit();

    QLabel *label_1 = new QLabel("Source Repo URL:");
    QLabel *label_2 = new QLabel("Target Repo URL:");
    QLabel *label_3 = new QLabel("Official Website:");
    QLabel *label_4 = new QLabel("Official Discord URL:");
    QLabel *label_5 = new QLabel("Official Discord Name:");
    QLabel *label_6 = new QLabel("Repo Description");
    QLabel *label_7 = new QLabel("Resulting HTML Code:");

    lineEdit_1 = new QLineEdit();
    lineEdit_2 = new QLineEdit();
    lineEdit_3 = new QLineEdit();
    lineEdit_4 = new QLineEdit();
    lineEdit_5 = new QLineEdit();
    lineEdit_6 = new QLineEdit();

    lineEdit_2->setPlaceholderText("Leave blank if repo name of this fork remains unchanged"); // for Target Repo URL
    lineEdit_3->setPlaceholderText("If exists"); // for Official Website
    lineEdit_4->setPlaceholderText("If exists"); // for Official Discord URL



    QGridLayout *layout = new QGridLayout;
    layout->addWidget(label_1, 0, 0);
    layout->addWidget(label_2, 1, 0);
    layout->addWidget(label_3, 2, 0);
    layout->addWidget(label_4, 3, 0); // Discord URL (on one row , 0th col)
    layout->addWidget(label_5, 3, 2); // Discord Name (same row , 2nd col)
    layout->addWidget(label_6, 4, 0);
    layout->addWidget(label_7, 5, 0);
    layout->addWidget(aboutButton, 6, 0); //

    layout->addWidget(lineEdit_1, 0, 1, 1 ,3);
    layout->addWidget(lineEdit_2, 1, 1, 1 ,3);
    layout->addWidget(lineEdit_3, 2, 1, 1 ,3);
    layout->addWidget(lineEdit_4, 3, 1, 1, 1); // Discord URL field (on one row, 1st col)
    layout->addWidget(lineEdit_5, 3, 3, 1, 1); // Discord URL (on one row , 3rd col)
    layout->addWidget(lineEdit_6, 4, 1, 1 ,3);
    layout->addWidget(resultText, 5, 1, 1 ,3);
    layout->addWidget(generateButton, 6, 1, 1, 3);

    setLayout(layout);

    // make enter key work within input fields
    connect(lineEdit_1, SIGNAL(returnPressed()), generateButton, SIGNAL(clicked()));
    connect(lineEdit_2, SIGNAL(returnPressed()), generateButton, SIGNAL(clicked()));
    connect(lineEdit_3, SIGNAL(returnPressed()), generateButton, SIGNAL(clicked()));
    connect(lineEdit_4, SIGNAL(returnPressed()), generateButton, SIGNAL(clicked()));
    connect(lineEdit_5, SIGNAL(returnPressed()), generateButton, SIGNAL(clicked()));
    connect(lineEdit_6, SIGNAL(returnPressed()), generateButton, SIGNAL(clicked()));


    connect(generateButton, SIGNAL(clicked()), this, SLOT(generateHTML()));
    connect(aboutButton, SIGNAL(clicked()), this, SLOT(showAboutDialog()));
}


Widget::~Widget()
{

}


void Widget::generateHTML()
{
    QString githubURL = "https://github.com/";
    QString sourceforgeURL = "https://sourceforge.net/p/";
    QString gitlabURL = "https://gitlab.com/";
    QString sourceRepo = lineEdit_1->text();
    QString targetRepo = lineEdit_2->text();
    QString officialSite = lineEdit_3->text();
    QString discordURL = lineEdit_4->text();
    QString discordName = lineEdit_5->text();
    QString descriptionText = lineEdit_6->text();

    QString shortSourceRepo = sourceRepo; // same by default
    QString shortTargetRepo = targetRepo;
    QString shortOfficialSite = officialSite;

    QString repoName, newRepoName, gitTags, sourceforgeTag, gitlabTag, resultHTML;

    QRegExp rx1(".*/(.*)$"); // regexp for getting repo name from relative path


    QString HTMLBeginning = "<div class=\"item\">\n<h3><a href=\"@01\">@02</a> @03[@04]</h3>\n";
    QString HTMLGithub = "<span class=\"subtext\">Mirrors:\n<a href=\"@05\">@06</a>,\n"
                         "<a href=\"https://techgaun.github.io/active-forks/index.html#@04\">Other forks</a>\n</span><br>\n";
    QString HTMLOtherGit = "<span class=\"subtext\">Unofficial mirror on github: \n<a href=\"@05\">@06</a></span><br>\n";
    QString HTMLOfficialSite = "<span class=\"subtext\">Official website: <a href=\"@07\">@08</a>";
    QString HTMLOfficialDiscord = "Official Discord channel: <a href=\"@09\">@10</a></span><br>\n";

    QString HTMLEnd = "<p>@11</p>\n</div>";


    // ****************************************************************************


    // Source Repo field

    if (sourceRepo.startsWith("http://") || sourceRepo.startsWith("https://")) { // if its full URL, not relative path

        if (sourceRepo.startsWith(githubURL) || sourceRepo.startsWith(gitlabURL)) { // if its github or gitlab
            if (sourceRepo.startsWith(githubURL)) { // for github
                shortSourceRepo.replace(githubURL, "");
            } else { // for gitlab
                shortSourceRepo.replace(gitlabURL, "");
                gitlabTag = "@gitlab "; // adding gitlab tag
            }

            if (rx1.indexIn(shortSourceRepo) != -1) { // if relative short path (shortSourceRepo) is correct
                repoName = rx1.cap(1); // getting result of regexp capture

            }  else { // relative short path (shortSourceRepo) is not correct
                resultHTML = "ERROR Please enter full Source repo URL or relative path for github in this format: user/repository\n"
                             "Parent directories like gitlab.com/username is not supported";
            }

        } else if(sourceRepo.startsWith(sourceforgeURL)) { // if it's sourceforge
            QRegExp rx2("/code/.*");
            shortSourceRepo.replace(sourceforgeURL, ""); // cut URL prefix
            shortSourceRepo.replace(rx2, ""); // cut everything after repo name
            repoName = shortSourceRepo; // for sourceforge - name is same as short relative path
            sourceforgeTag = "@sourceforge ";

        } else if (sourceRepo.startsWith("https://sourceforge.net/")) { // sourceforge without /p/ in URL means it's not direct link to repo code
            resultHTML = "ERROR Please enter direct link to repository code, not to main page. Example:\n"
                         "https://sourceforge.net/p/quark/code/HEAD/tree/\n"
                         "Instead of https://sourceforge.net/projects/quark/ ";

        } else {
            resultHTML = "ERROR Unrecognized git service. Only github, sourceforge and gitlab are supported. For other fit services - do it manually";

        }


    } else if (!sourceRepo.isEmpty()) { // if this field is not empty


        if (rx1.indexIn(shortSourceRepo) != -1) { // if relative short path (shortSourceRepo) is correct
            repoName = rx1.cap(1); // getting result of regexp capture
            sourceRepo = githubURL + sourceRepo; // make full URL
        } else {
            resultHTML = "ERROR Please enter full source URL or relative path for github in this format: user/repository";
        }

    } else { // if this field is empty
        resultHTML = "Please enter something for source repo URL";
    }


    // ****************************************************************************

    // Target Repo field


    if (!targetRepo.isEmpty()) { // if this field not empty
        if (!targetRepo.startsWith(githubURL)) { // if its relative path (not full URL)

            if (rx1.indexIn(shortTargetRepo) != -1) { // if relative short path (shortTargetRepo) is correct
                newRepoName = rx1.cap(1);

            } else { // otherwise - assume its just name of repo
                newRepoName = targetRepo;
            }
            shortTargetRepo = "QuakeEngines/" + newRepoName;
            targetRepo = githubURL + shortTargetRepo; // make full URL

        } else { // for full URL
            shortTargetRepo = shortTargetRepo.replace(githubURL, "");
        }

    } else  { // if this field is empty
        if (sourceRepo.startsWith(githubURL)) { // for github
            shortTargetRepo = "QuakeEngines"; // don't show repo name in relative path (shortTargetRepo) because it's same as original
            targetRepo = githubURL + "QuakeEngines/" + repoName;

        } else { // for other git services - show repo name just for clarity
            shortTargetRepo = "QuakeEngines/" + repoName;
            targetRepo = githubURL + shortTargetRepo;
        }
    }

    // ****************************************************************************

    // Official Website field

    if (!officialSite.isEmpty()) {

        if (officialSite.startsWith("https://") || officialSite.startsWith("http://")) {
            shortOfficialSite.replace("https://", "");
            shortOfficialSite.replace("http://", "");
        } else {
            officialSite = "http://" + officialSite; // add http:// if not exist
        }
        shortOfficialSite.replace(QRegExp("/$"), ""); // remove last slash in URL for shortOfficialSite

        if (!discordURL.isEmpty()) { // if Official Discord also exist
            HTMLOfficialSite += " /\n"; // add slash and newline symbol in the end of HTML
        } else {
            HTMLOfficialSite += "\n</span><br>\n"; // if not - close span tag
        }

    } else {
        HTMLOfficialSite = ""; // if officialSite field is empty - make HTMLOfficialSite blank too
    }


    // ****************************************************************************

    // Official Discord field

    if (!discordURL.isEmpty()) {
        if (officialSite.isEmpty()) { // if Official Website field is empty -
            HTMLOfficialDiscord = "<span class=\"subtext\">" + HTMLOfficialDiscord; // add opening span tag in the beginning of HTML
        }

    } else {
        HTMLOfficialDiscord = ""; // if officialDiscord field is empty - make HTMLOfficialDiscord blank too
    }

    if (!discordURL.isEmpty() && discordName.isEmpty()) { // if discord URL is not empty, but name field is
        resultHTML = "ERROR Please enter name of the Official Discord channel";
    }


    // ****************************************************************************

    // Description field

    descriptionText.replace(QRegExp(" +"), " "); // remove double spaces

    // ****************************************************************************

    // Construct resulting HTML code

    if (resultHTML.isEmpty()) { // if resultHTML it not containing some error message

        if (sourceRepo.startsWith(githubURL)) { // if source repo is on github
            resultHTML = HTMLBeginning + HTMLGithub + HTMLOfficialSite + HTMLOfficialDiscord + HTMLEnd;
        } else {
            resultHTML = HTMLBeginning + HTMLOtherGit + HTMLOfficialSite + HTMLOfficialDiscord + HTMLEnd;
        }
    }


    // ****************************************************************************

    // Replacing placeholders in template with real values

    gitTags = sourceforgeTag + gitlabTag; // one of the tag wouldn't be blank (both are blank if its github)

    resultHTML.replace("@01", sourceRepo);
    resultHTML.replace("@02", repoName);
    resultHTML.replace("@03", gitTags);
    resultHTML.replace("@04", shortSourceRepo);
    resultHTML.replace("@05", targetRepo);
    resultHTML.replace("@06", shortTargetRepo);
    resultHTML.replace("@07", officialSite);
    resultHTML.replace("@08", shortOfficialSite);
    resultHTML.replace("@09", discordURL);
    resultHTML.replace("@10", discordName);
    resultHTML.replace("@11", descriptionText);

    resultText->setPlainText(resultHTML);

}



void Widget::showAboutDialog()
{
    AboutDialog *aboutDialog = new AboutDialog(this);
    aboutDialog->resize(180, 100);
    aboutDialog->exec();
}
