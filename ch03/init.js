var GROOGNA_ORIGIN = "http://localhost:10041";
var CONTENT_TABLE = "Documents";
var INDEX_TABLE = "Terms";

document.addEventListener("DOMContentLoaded", function() {
    tableList()
        .then(function(list) {
            var contentTableExists = (list.indexOf(CONTENT_TABLE) !== -1);
            var indexTableExists = (list.indexOf(INDEX_TABLE) !== -1);

            return (contentTableExists && indexTableExists);
        })
        .then(function(tableExists) {
            if (tableExists) {
                showMessage("必要なテーブル（" + CONTENT_TABLE + ", " + INDEX_TABLE + "）は作成済みです。");
            } else {
                var button = document.querySelector("button");
                button.addEventListener("click", createTables);
                button.disabled = false;
            }
        })
        .catch(function(error) {
            console.error(error);
            alert(error);
        });
});

function createTables() {
    var button = document.querySelector("button");
    button.disabled = true;

    return createDocumentsTable()
        .then(createTermsTable)
        .then(function() {
            showMessage("必要なテーブル（" + CONTENT_TABLE + ", " + INDEX_TABLE + "）を作成しました");
        })
        .catch(function(error) {
            button.disabled = false;
            console.error(error);
            alert(error);
        });
}

function tableList() {
    var endPoint = "/d/table_list";

    return sendGetRequest(endPoint)
        .then(function(response) {
            var status = response.shift();
            var body = response.shift();
            var scheme = body.shift();
            var nameFieldIndex = scheme.findIndex(function(nameAndType) {
                var fieldName = nameAndType[0];
                var fieldType = nameAndType[1];

                return (fieldName === "name");
            });
            var list = body.map(function(record) {
                return record[nameFieldIndex];
            });

            return list;
        });
}

function createDocumentsTable() {
    var endPoint = "/d/table_create";
    var params = {
        name: CONTENT_TABLE,
        flags: "TABLE_HASH_KEY"
    };

    return sendGetRequest(endPoint, params)
        .then(function() {
            var endPoint = "/d/column_create";
            var params = {
                table: CONTENT_TABLE,
                name: "title",
                type: "ShortText"
            };

            return sendGetRequest(endPoint, params);
        })
        .then(function() {
            var endPoint = "/d/column_create";
            var params = {
                table: CONTENT_TABLE,
                name: "content",
                type: "LongText"
            };

            return sendGetRequest(endPoint, params);
        });
}

function createTermsTable() {
    var endPoint = "/d/table_create";
    var params = {
        name: INDEX_TABLE,
        flags: "TABLE_PAT_KEY",
        key_type: "ShortText",
        default_tokenizer: "TokenBigram",
        normalizer: "NormalizerAuto"
    };

    return sendGetRequest(endPoint, params)
        .then(function() {
            var endPoint = "/d/column_create";
            var params = {
                table: INDEX_TABLE,
                name: "content_terms",
                flags: "COLUMN_INDEX|WITH_POSITION",
                type: CONTENT_TABLE,
                source: "content"
            };

            return sendGetRequest(endPoint, params);
        });
}

function sendGetRequest(endPoint, params) {
    return new Promise(function(resolve, reject) {
        var url = GROOGNA_ORIGIN + endPoint + "?command_version=2";

        if (params) {
            for (var key in params) {
                var value = params[key];
                url += "&" + encodeURIComponent(key) + "=" + encodeURIComponent(value);
            }
        }

        var request = new XMLHttpRequest();
        request.onload = function(event) {
            var data = event.target.response;
            resolve(JSON.parse(data));
        };
        request.onerror = reject;
        request.open("GET", url);
        request.send();
    });
}

function showMessage(message) {
    var p = document.createElement("p");
    p.textContent = message;
    document.body.appendChild(p);
}
