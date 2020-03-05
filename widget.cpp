#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("Simple QuakeEngine HTML generator");


    // make enter key work within input fields
    connect(ui->lineEdit, SIGNAL(returnPressed()), ui->generateButton, SIGNAL(clicked()));
    connect(ui->lineEdit_2, SIGNAL(returnPressed()), ui->generateButton, SIGNAL(clicked()));
    connect(ui->lineEdit_3, SIGNAL(returnPressed()), ui->generateButton, SIGNAL(clicked()));
    connect(ui->lineEdit_4, SIGNAL(returnPressed()), ui->generateButton, SIGNAL(clicked()));
}


Widget::~Widget()
{
    delete ui;
}


void Widget::on_generateButton_clicked()
{
    QString githubURL = "https://github.com/";
    QString sourceforgeURL = "https://sourceforge.net/p/";
    QString gitlabURL = "https://gitlab.com/";
    QString sourceRepo = ui->lineEdit->text();
    QString targetRepo = ui->lineEdit_2->text();
    QString officialSite = ui->lineEdit_3->text();
    QString descriptionText = ui->lineEdit_4->text();

    QString shortSourceRepo = sourceRepo; // same by default
    QString shortTargetRepo = targetRepo;
    QString shortOfficialSite = officialSite;

    QString repoName, newRepoName, gitTags, sourceforgeTag, gitlabTag, resultHTML;

    QRegExp rx1(".*/(.*)$"); // regexp for getting repo name from relative path


    QString HTMLBeginning = "<div class=\"item\">\n<h3><a href=\"@1\">@2</a> @3[@4]</h3>\n";
    QString HTMLGithub = "<span class=\"subtext\">Mirrors: <a href=\"@5\">@6</a>,\n"
                         "<a href=\"https://techgaun.github.io/active-forks/index.html#@4\">Other forks</a></span>\n";
    QString HTMLOtherGit = "<span class=\"subtext\">Unofficial mirror on github: \n<a href=\"@5\">@6</a></span><br>\n";
    QString HTMLOfficialSite = "<span class=\"subtext\">Official website: <a href=\"@7\">@8</a>\n</span>\n";
    QString HTMLEnd = "<p>@9</p>\n</div>";


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
    } else {
        HTMLOfficialSite = ""; // if officialSite field is empty - make HTMLOfficialSite blank too
    }


    // ****************************************************************************

    // Description field

    descriptionText.replace(QRegExp(" +"), " "); // remove double spaces

    // ****************************************************************************

    // Construct resulting HTML code

    if (resultHTML.isEmpty()) { // if resultHTML it not containing some error message

        if (sourceRepo.startsWith(githubURL)) { // if source repo is on github
            resultHTML = HTMLBeginning + HTMLGithub + HTMLOfficialSite + HTMLEnd;
        } else {
            resultHTML = HTMLBeginning + HTMLOtherGit + HTMLOfficialSite + HTMLEnd;
        }
    }


    // ****************************************************************************

    // Replacing placeholders in template with real values

    gitTags = sourceforgeTag + gitlabTag; // one of the tag wouldn't be blank (both are blank if its github)

    resultHTML.replace("@1", sourceRepo);
    resultHTML.replace("@2", repoName);
    resultHTML.replace("@3", gitTags);
    resultHTML.replace("@4", shortSourceRepo);
    resultHTML.replace("@5", targetRepo);
    resultHTML.replace("@6", shortTargetRepo);
    resultHTML.replace("@7", officialSite);
    resultHTML.replace("@8", shortOfficialSite);
    resultHTML.replace("@9", descriptionText);

    ui->resultText->setPlainText(resultHTML);

}



void Widget::on_aboutButton_clicked()
{
    AboutDialog *aboutDialog = new AboutDialog(this);
    aboutDialog->resize(180, 100);
    aboutDialog->exec();
}
