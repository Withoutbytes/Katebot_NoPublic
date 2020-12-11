const fs = require("fs");
const path = require("path");

const basePath = path.join(__dirname, "src");

const GetFilesInSubFolders = (_path) => {
    const files = [];
    const dirs = fs.readdirSync(_path, {
        encoding: "utf-8"
    });
    for (const pathRel of dirs) {
        const tempPath = path.join(_path, pathRel);
        if (fs.lstatSync(tempPath).isDirectory()) files.push(...GetFilesInSubFolders(tempPath));
        if (fs.lstatSync(tempPath).isFile()) files.push(tempPath);
    }
    return files;
}


const FindString = (rawFile, index) => {
    let beginOfString = rawFile.indexOf("\"", index);
    if (beginOfString == -1) return undefined;
    let endOfString = rawFile.indexOf("\"", beginOfString+1);
    return {begin: beginOfString, end: endOfString+1};
}

const GetAllStringsInFile = (rawFile) => {

    let strings = [];
    let i = 0;
    while (i < rawFile.length) {
        let found = FindString(rawFile, i);
        if (!found) break;
        strings.push(found);
        i = found.end + 1;
    }
    return strings;
}

for (const file of GetFilesInSubFolders(basePath)) {
    const rawFile = fs.readFileSync(file, {encoding: "utf-8"});
    let copyRawFile = rawFile;
    console.log(file);
    let i = 0;
    const stringsReplaced = [];
    for (const str of GetAllStringsInFile(rawFile)) {
        i = str.end;
        const include = "#include ";
        if (rawFile.substr(str.begin-include.length, include.length) == include)
             continue; // do not use string in sameline of include

        let string = rawFile.substring(str.begin, str.end);
        if (!stringsReplaced.includes(string)) {
            copyRawFile = copyRawFile.replace(string, `xorstr(${string}).crypt_get()`);
            stringsReplaced.push(string);
        }
    }
    // console.log(copyRawFile);
    fs.writeFileSync(file, copyRawFile, {encoding: "utf-8"});
}