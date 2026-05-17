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
    PRINT,
    PREFIX
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
    fprintf(stderr, "Parsing command \"%s\"...\n", command);
    if (StringComparer.Ordinal.Equals(command, "ADD")) {
        typeof(char[1024]) id_buf;
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
    } else if (StringComparer.Ordinal.Equals(command, "PREFIX")) {
        typeof(char[1024]) prefix_buf;
        fscanf(sys->input, "%s", prefix_buf);
        return (Cmd) {
            .cmd_id = PREFIX,
            .keyword = new(string)(prefix_buf)
        };
    } else throw new(Exception)(string_Format("Unknown command: %s", command));
    return default(Cmd);
}

int main(int argc, char** argv)
{
    System sys = {(struct tag_System) {
        .Files = new(DoublyLinkedList(File))(),
        .Keywords = new(RetrievalTree)(),
        .input = fopen("indexare.in", "rt"),
        .output = fopen("indexare.out", "wt")
    }};
    if (argc > 1 && StringComparer.Ordinal.Equals(argv[1], "-d")) {
        fprintf(stderr, "INFO: Running in debug mode!\n");
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
                List_string_Destroy(&cmd.file_keywords);
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
            case FIND:
                Find(sys, cmd.keyword);
                break;
            case TOPK:
                TopK(sys, cmd.keyword, cmd.count);
                break;
            case PRINT:
                Print(sys);
                break;
            case PREFIX:
                Prefix(sys, cmd.keyword);
                break;
            }
        }
    }
    catch (Exception ex) {
        fprintf(stderr, "Exception thrown: ");
        fprintf(stderr, "%s\n", ex->Message);
        fprintf(stderr, "Program interrupted.\n");
        return 1;
    }
    return 0;
}
