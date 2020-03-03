#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("Simple QuakeEngine HTML generator");


    // make enter key work within input fields
    connect(ui->lineEdit, SIGNAL(returnPressed()),ui->pushButton,SIGNAL(clicked()));
    connect(ui->lineEdit_2, SIGNAL(returnPressed()),ui->pushButton,SIGNAL(clicked()));
}


Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    QString githubURL = "https://github.com/";
    QString sourceforgeURL = "https://sourceforge.net/p/";
    QString gitlabURL = "https://gitlab.com/";
    QString sourceRepo = ui->lineEdit->text();
    QString targetRepo = ui->lineEdit_2->text();
    QString descriptionText = ui->descriptionText->toPlainText();

    QString shortSourceRepo = sourceRepo; // same by default
    QString shortTargetRepo = targetRepo;

    QString repoName, newRepoName, gitTags, sourceforgeTag, gitlabTag, resultHTML;

    QRegExp rx1(".*/(.*)$"); // regexp for getting repo name from relative path


    QString HTMLBeginning = "<div class=\"item\">\n<h3><a href=\"@1\">@2</a> @3[@4]</h3>\n";
    QString HTMLGithub = "<span class=\"subtext\">Mirrors: <a href=\"@5\">@6</a>,\n"
                         "<a href=\"https://techgaun.github.io/active-forks/index.html#@4\">Other forks</a></span>\n";
    QString HTMLOtherGit = "<span class=\"subtext\">Unofficial mirror on github: <a href=\"@5\">@6</a></span>\n";
    QString HTMLEnd = "<p>@7</p>\n</div>";


    // ****************************************************************************


    // Source Repo field

    if (sourceRepo.startsWith("http://") || sourceRepo.startsWith("https://")) { // if its full URL, not relative path

        if (sourceRepo.startsWith(githubURL) || sourceRepo.startsWith(gitlabURL)) { // if its github or gitlab
            if (sourceRepo.startsWith(githubURL)) { // for github
                shortSourceRepo = shortSourceRepo.replace(githubURL, "");
            } else { // for gitlab
                shortSourceRepo = shortSourceRepo.replace(gitlabURL, "");
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
            shortSourceRepo = shortSourceRepo.replace(sourceforgeURL, ""); // cut URL prefix
            shortSourceRepo = shortSourceRepo.replace(rx2, ""); // cut everything after repo name
            repoName = shortSourceRepo; // for sourceforge - name is same as short relative path
            sourceforgeTag = "@sourceforge ";

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

    // Construct resulting HTML code

    if (resultHTML.isEmpty()) { // if resultHTML it not containing some error message

        if (sourceRepo.startsWith(githubURL)) { // if source repo is on github
            resultHTML = HTMLBeginning + HTMLGithub + HTMLEnd;
        } else {
            resultHTML = HTMLBeginning + HTMLOtherGit + HTMLEnd;
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
    resultHTML.replace("@7", descriptionText);

    ui->plainTextEdit->setPlainText(resultHTML);

}



void Widget::on_pushButton_2_clicked()
{
    AboutDialog *aboutDialog = new AboutDialog(this);
    aboutDialog->resize(180, 100);
    aboutDialog->exec();
}
