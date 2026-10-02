# Documentation website

The site uses Doxygen for both the `docs/` Markdown files and the project's
C++ API comments. It uses the sidebar layout of
[Doxygen Awesome CSS](https://github.com/jothepro/doxygen-awesome-css), pinned
to v2.5.0. Its configuration and landing page live in this folder; generated
files go to `website/_site/`.

From the repository root, install Doxygen and CMake, then build and view the site:

```sh
cmake -P website/fetch_theme.cmake
doxygen website/Doxyfile
python3 -m http.server 8000 --directory website/_site
```

Open <http://localhost:8000/>. Doxygen renders standard Markdown features
such as headings, links, lists, tables, and fenced code blocks. Advanced
GitHub-only features, such as Mermaid diagrams, appear as code blocks.
The theme download requires network access on first use. Its two CSS files are
cached in the ignored `website/_theme/` directory and verified by SHA-256.

The GitHub Pages workflow builds the same site on pushes to `main` and `dev`
and on pull requests targeting `main`. It publishes after a push to `main`,
including a merged pull request. To enable publication,
set **Settings → Pages → Build and deployment → Source** to **GitHub Actions**
in the repository.
