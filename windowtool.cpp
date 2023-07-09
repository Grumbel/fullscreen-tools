#include <charconv>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <fmt/format.h>
#include <gtkmm.h>
#include <gdkmm/window.h>

struct Size
{
  int w;
  int h;
};

struct Position
{
  int x;
  int y;
};

bool parse_bool(std::string const& text)
{
  return bool(std::stoi(text));
}

unsigned int parse_uint(std::string_view text)
{
  unsigned int value;
  std::from_chars_result err;
  if (text.starts_with("0x")) {
    err = std::from_chars(text.data() + 2, text.data() + text.size(), value, 16);
  } else {
    err = std::from_chars(text.data(), text.data() + text.size(), value);
  }

  if (err.ec != std::errc()) {
    throw std::runtime_error(fmt::format("failed to parse: {}", text));
  }

  return value;
}

Position parse_position(std::string text)
{
  Position position;

  if (sscanf(text.c_str(), "%dx%d", &position.x, &position.y) == 2)
  {
    return position;
  } else {
    throw std::runtime_error(fmt::format("failed to convert '{}' to position", text));
  }
}

Size parse_size(std::string text)
{
  Size size;

  if (sscanf(text.c_str(), "%dx%d", &size.w, &size.h) == 2 ||
      sscanf(text.c_str(), "%d,%d", &size.w, &size.h) == 2)
  {
    return size;
  } else {
    throw std::runtime_error(fmt::format("failed to convert '{}' to size", text));
  }
}

struct Options
{
  std::optional<bool> override_redirect = {};
  std::optional<bool> decorations = {};
  std::optional<bool> keep_above = {};
  std::optional<bool> keep_below = {};
  std::optional<Size> size = {};
  std::optional<Position> position = {};
  std::optional<unsigned int> window = {};
  bool verbose = false;
};

void print_help(std::string_view arg0)
{
  fmt::print(
    "Usage: {} [WINDOWID]\n"
    "Manipulate window properties in X11.\n"
    "\n"
    "Options:\n"
    "  --decorations     Enable decorations\n"
    "  --no-decorations  Disable decorations\n"
    "  --keep-above      Enable keep-above\n"
    "  --no-keep-above   Disable keep-above\n"
    "  --keep-below      Enable keep-below\n"
    "  --no-keep-below   Disable keep-below\n"
    "  --move X,Y        Move window to X,Y\n"
    "  --resize XxY      Resize window to WxH\n",
    arg0);
}

Options parse_args(int argc, char** argv)
{
  Options opts;

  for (int i = 1; i < argc; ++i)
  {
    std::string_view arg = argv[i];

    if (!arg.empty() && arg[0] == '-') {
      if (arg == "--help" || arg == "-h") {
        print_help(argv[0]);
        exit(EXIT_SUCCESS);
      } else if (arg == "--override-redirect" || arg == "-o") {
        opts.override_redirect = true;
      } else if (arg == "--no-override-redirect" || arg == "-O") {
        opts.override_redirect = false;
      } else if (arg == "--decorations" || arg == "-d") {
        opts.decorations = true;
      } else if (arg == "--no-decorations" || arg == "-D") {
        opts.decorations = false;
      } else if (arg == "--keep-above" || arg == "-a") {
        opts.keep_above = true;
      } else if (arg == "--no-keep-above" || arg == "-A") {
        opts.keep_above = false;
      } else if (arg == "--keep-below" || arg == "-b") {
        opts.keep_below = true;
      } else if (arg == "--no-keep-below" || arg == "-B") {
        opts.keep_below = false;
      } else if (arg == "--move" || arg == "-m") {
        i += 1;
        opts.position = parse_position(std::string(argv[i]));
      } else if (arg == "--resize" || arg == "-r") {
        i += 1;
        opts.size = parse_size(std::string(argv[i]));
      } else {
        throw std::runtime_error(fmt::format("unknown argument: {}", arg));
      }
    } else {
      opts.window = parse_uint(arg);
    }
  }

  return opts;
}

int main(int argc, char** argv)
{
  Options opts = parse_args(argc, argv);

  Gtk::Main kit(argc, argv);

  Glib::RefPtr<Gdk::Window> w;
  if (opts.window) {
    // can't find foreign_new() in Gtkmm
    fmt::print("using window {}\n", *opts.window);
    GdkWindow* win = gdk_window_foreign_new(*opts.window);
    Glib::RefPtr<Gdk::Drawable> drawable(Glib::wrap(win));
    w = Glib::RefPtr<Gdk::Window>::cast_dynamic(drawable);
  } else {
    fmt::print("using active window\n");
    w = Gdk::Screen::get_default()->get_active_window();
  }

  if (!w) {
    throw std::runtime_error(fmt::format("invalid window id: {}", *opts.window));
  }

  if (opts.keep_above) {
    fmt::print("Setting keep_above: {}\n", *opts.keep_above);
    w->set_keep_above(*opts.keep_above);
  }

  if (opts.decorations) {
    fmt::print("Setting decorations: {}\n", *opts.decorations);

    if (*opts.decorations) {
      Glib::usleep(1000000);
      w->set_decorations(Gdk::DECOR_ALL);
    } else {
      Glib::usleep(1000000);
      w->set_decorations(Gdk::WMDecoration{});
    }
  }

  if (opts.override_redirect) {
    w->set_override_redirect(*opts.override_redirect);
    Gdk::flush();
    Glib::usleep(500000);
  }

  if (opts.position) {
    Glib::usleep(1000000);
    w->move(opts.position->x, opts.position->y);
  }

  if (opts.size) {
    Glib::usleep(1000000);
    w->resize(opts.size->w, opts.size->h);
  }

  if (opts.keep_below) {
    fmt::print("Setting keep_below: {}\n", *opts.keep_below);
    w->set_keep_below(*opts.keep_below);
  }

  fmt::print("flush\n");
  Gdk::flush();

  return 0;
}

/* EOF */
