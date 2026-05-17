#include "google.h"
#include "retree.h"
#include <stdio.h>

enum tag_CmdID {
    ADD,
    DEL,
    ADDKW,
    DELKW,
    FIND,
    TOPK,
    PRINT
};
typedef struct tag_Cmd {
    enum tag_CmdID cmd_id;
    struct {
        string file_id;
        union {
            struct {
                int file_score;
                List(string) file_keywords;
            };
            string keyword;
        };
    };
    int count;
} Cmd;

Cmd parse_cmd(System sys, string command)
{
    if (StringComparer.Ordinal.Equals(command, "ADD")) {
        char id_buf[1024];
        int score, count;
        fscanf(sys->input, "%s %d %d ", id_buf, &score, &count);
        List(string) kws = new(List(string))(count);
        for (int i = 0; i < count; ++i) {
            char kw_buf[1024];
            fscanf(sys->input, "%s ", kw_buf);
            List_string_Add(kws, new(string)(kw_buf));
        }
        return (Cmd) {
            .cmd_id = ADD,
            .file_id = new(string)(id_buf),
            .file_score = score,
            .file_keywords = kws
        };
    } else if (StringComparer.Ordinal.Equals(command, "DEL")) {
        typeof(char[1024]) id_buf;
        fscanf(sys->input, "%s", id_buf);
        return (Cmd) {
            .cmd_id = DEL,
            .file_id = new(string)(id_buf)
        };
    } else if (StringComparer.Ordinal.Equals(command, "ADDKW")) {
        typeof(char[1024]) id_buf, kw_buf;
        fscanf(sys->input, "%s %s", id_buf, kw_buf);
        return (Cmd) {
            .cmd_id = ADDKW,
            .file_id = new(string)(id_buf),
            .keyword = new(string)(kw_buf)
        };
    } else if (StringComparer.Ordinal.Equals(command, "DELKW")) {
        typeof(char[1024]) id_buf, kw_buf;
        fscanf(sys->input, "%s %s", id_buf, kw_buf);
        return (Cmd) {
            .cmd_id = DELKW,
            .file_id = new(string)(id_buf),
            .keyword = new(string)(kw_buf)
        };
    } else if (StringComparer.Ordinal.Equals(command, "FIND")) {
        typeof(char[1024]) kw_buf;
        fscanf(sys->input, "%s", kw_buf);
        return (Cmd) {
            .cmd_id = FIND,
            .keyword = new(string)(kw_buf)
        };
    } else if (StringComparer.Ordinal.Equals(command, "TOPK")) {
        typeof(char[1024]) kw_buf;
        int k;
        fscanf(sys->input, "%s %d", kw_buf, &k);
        return (Cmd) {
            .cmd_id = TOPK,
            .keyword = new(string)(kw_buf),
            .count = k
        };
    } else if (StringComparer.Ordinal.Equals(command, "PRINT")) {
        return (Cmd) {
            .cmd_id = PRINT
        };
    } else throw(new(Exception)(string_Format("Unknown command: %s", command)));
}

int main(int argc, char** argv)
{
    System sys = {(struct tag_System) {
        .Files = new(DoublyLinkedList(File))(),
        .Keywords = memalloc(RetrievalTree),
        .input = fopen("indexare.in", "rt"),
        .output = fopen("indexare.out", "wt")
    }};
    if (argc > 1 && StringComparer.Ordinal.Equals(argv[1], "-d")) {
        printf("Running in debug mode!\n");
        sys->input = stdin;
        sys->output = stdout;
    }
    int c;
    fscanf(sys->input, "%d", &c);
    try {
        for (int i = 0; i < c; ++i) {
            char command_buf[32];
            fscanf(sys->input, "%s", command_buf);
            Cmd cmd = parse_cmd(sys, command_buf);
            switch (cmd.cmd_id) {
            case ADD:
                if (Add(sys, cmd.file_id, cmd.file_score, (IEnumerable(string))cmd.file_keywords)) fprintf(sys->output, "OK\n");
                else fprintf(sys->output, "EXISTS\n");
                break;
            case DEL:
                if (Del(sys, cmd.file_id)) fprintf(sys->output, "OK\n");
                else fprintf(sys->output, "NOT FOUND\n");
                break;
            case ADDKW:
                if (AddKW(sys, cmd.file_id, cmd.keyword)) fprintf(sys->output, "OK\n");
                else fprintf(sys->output, "NOT FOUND\n");
                break;
            case DELKW:
                if (DelKW(sys, cmd.file_id, cmd.keyword)) fprintf(sys->output, "OK\n");
                else fprintf(sys->output, "NOT FOUND\n");
                break;
            case FIND: try {
                    Heap(File) files = Find(sys, cmd.keyword);
                    File f;
                    fprintf(sys->output, "%d ", files->Count);
                    while (Heap_File_TryPop(files, &f)) {
                        fprintf(sys->output, "%s ", f->ID);
                    }
                    fprintf(sys->output, "\n");
                } catch (Exception ex) {
                    if (StringComparer.Ordinal.Equals(ex->Message, "ERR_NOT_TERMINAL_KW")) {
                        fprintf(sys->output, "EMPTY\n");
                    }
                }
                break;
            case TOPK: do {
                    Heap(File) files = TopK(sys, cmd.keyword);
                    File f;
                    while ((cmd.count -= 1) >= 0 && Heap_File_TryPop(files, &f)) {
                        fprintf(sys->output, "%s ", f->ID);
                    }
                    fprintf(sys->output, "\n");
                } while (0);
                break;
            case PRINT:
                Print(sys);
                break;
            }
        }
    }
    catch (Exception ex) {
        fprintf(stderr, "%s\n", ex->Message);
        fprintf(stderr, "Program interrupted: exception thrown\n");
        return 1;
    }
    return 0;
}
