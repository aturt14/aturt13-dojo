#include <algorithm>
#include <array>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>

// #define RATING

struct Article {
    uint64_t likes;
    uint64_t dislikes;
    std::string name;
    std::string_view content;
    Article(const std::string& name_, const std::string& content_): name(name_), content(content_) {}
};

class Roskomnadzor {
public:
    void check_article(Article* article_stupid) const { // I don't like those C-style pointers, better get rid of them as fast as possible
        std::unique_ptr<Article> article_smart{article_stupid};
        std::string_view content = article_smart->content;
        for (const auto& bad_word: m_reject_words) {
            if (content.find(bad_word) != std::string::npos) {
                std::cerr << "Journalist detected!" << std::endl;
                std::exit(-1);
            }
        }
    }
private:
    static constexpr std::array<std::string_view, 3> m_reject_words{
        "freedom", "peace", "demonstration"
    };
};


std::string get_article_name() {
    std::cout << "Enter article name:" << std::endl;
    std::string article_name;
    std::getline(std::cin, article_name);
    return article_name;
}

Article* get_article_by_name(std::vector<std::unique_ptr<Article>>& articles, std::string_view name) {
    auto it = std::find_if(articles.begin(), articles.end(), [&](const std::unique_ptr<Article>& a) {
        return (a->name == name);
    });
    if (it != articles.end()) {
        return it->get();
    }
    std::cerr << "You would have to write that article first." << std::endl;
    exit(-1);
}

void do_write_article(std::vector<std::unique_ptr<Article>>& articles) {
    std::string content, name;
    name = get_article_name();
    std::cout << "Write your article here:" << std::endl;
    std::getline(std::cin, content);
    articles.emplace_back(std::make_unique<Article>(name, content));
}

void do_publish_article(std::vector<std::unique_ptr<Article>>& articles) {
    std::string name = get_article_name();
    Roskomnadzor r;
    auto article = get_article_by_name(articles, name);
    r.check_article(article);
    std::cout << "Article " << name << " published successfully!" << std::endl;
}

void do_read_article(std::vector<std::unique_ptr<Article>>& articles) {
    auto article = get_article_by_name(articles, get_article_name());
    std::cout << "Title: " << article->name << '\n';
    std::cout << "Content: " << article->content << std::endl;
}

void do_like_article(std::vector<std::unique_ptr<Article>>& articles) {
#ifndef RATING
    std::cerr << "The like feature has been disabled!" << std::endl;
#endif
#ifdef RATING
    auto article = get_article_by_name(articles, get_article_name());
    article->likes++;
    std::cout << "Liked article" << article->name << std::endl;
#endif
}

void do_dislike_article(std::vector<std::unique_ptr<Article>>& articles) {
#ifndef RATING
    std::cerr << "The dislike feature has been disabled!" << std::endl;
#endif
#ifdef RATING
    auto article = get_article_by_name(articles, get_article_name());
    article->dislikes++;
    std::cout << "Liked article" << article->name << std::endl;
#endif
}

void show_article_stats(std::vector<std::unique_ptr<Article>>& articles) {
    // TODO: show likes and dislikes
}

void menu_loop() {
    std::string option;
    std::vector<std::unique_ptr<Article>> articles;
    while (true) {
        std::cout << "write_article/publish_article/read_article/like_article/dislike_article\n";
        std::cout << "Choose your option:" << std::endl;
        std::getline(std::cin, option);
        if (option == "write_article") {
            do_write_article(articles);
        }
        else if (option == "publish_article") {
            do_publish_article(articles);
        }
        else if (option == "read_article") {
            do_read_article(articles);
        }
        else if (option == "like_article") {
            do_like_article(articles);
        }
        else if (option == "dislike_article") {
            do_dislike_article(articles);
        }
        else return;
    }
}


int main(int argc, char* argv[]) {
    std::cout << "We have freedom of speech here!" << std::endl;
    menu_loop();

    return 0;
}
