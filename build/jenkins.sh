#!/bin/bash

function semverParseInto() {
    local RE='[^0-9]*\([0-9]*\)[.]\([0-9]*\)[.]\([0-9]*\)\([0-9A-Za-z-]*\)'
    #MAJOR
    eval $2=`echo $1 | sed -e "s#$RE#\1#"`
    #MINOR
    eval $3=`echo $1 | sed -e "s#$RE#\2#"`
    #MINOR
    eval $4=`echo $1 | sed -e "s#$RE#\3#"`
    #SPECIAL
    eval $5=`echo $1 | sed -e "s#$RE#\4#"`
}

cd "$(dirname "$0")"

$CXX --version
cmake --version

VERSION=$(git describe --tags HEAD~1 2>/dev/null)
if [[ ! -z "$VERSION" ]]; then
    semverParseInto "$VERSION" MAJOR MINOR PATCH SPECIAL

    MESSAGE=$(git log --oneline --format=%B -n1 HEAD | head -n 1)

    OLD_TAG="v$MAJOR.$MINOR.$PATCH"
    
    if [[ $MESSAGE == Release*:* ]]; then
        ((++MAJOR))
        MINOR=0
        PATCH=0
    elif [[ $MESSAGE == Feat*:* ]]; then
        ((++MINOR))
        PATCH=0
    elif [[ $MESSAGE == Fix*:* ]]; then
        ((++PATCH))
    fi
    
    NEW_TAG="v$MAJOR.$MINOR.$PATCH"
    
    if [ $OLD_TAG != $NEW_TAG ]; then
        git tag "$NEW_TAG" 2>/dev/null
    fi

    CHANGELOG=$(git log --oneline --format=%B $OLD_TAG..$NEW_TAG | sed -e ':a;N;$!ba;s/\n/\\n/g')
fi

cmake -G "Unix Makefiles" ..
make -l deb tgz

if [ $OLD_TAG != $NEW_TAG ]; then
    echo "Pushing tags"
    git push --tags
fi

GITHUB_API_URL="https://api.github.com/repos/benjinx/Temporality"

if [ $OLD_TAG != $NEW_TAG ]; then

    cat <<EOF > create_release.json
{
      "tag_name": "$NEW_TAG",
      "name": "$NEW_TAG",
      "body": "$CHANGELOG",
      "draft": false,
      "prerelease": false
}
EOF

    UPLOAD_URL=$(curl -u $GITHUB_AUTH \
        $GITHUB_API_URL/releases \
        -d @create_release.json \
        | jq -r .upload_url \
        | cut -d'{' -f1)

    DEB=$(ls -1 *.deb 2>/dev/null)
    if [[ ! -z "$DEB" ]]; then
        echo "Uploading $DEB"
        curl -u $GITHUB_AUTH \
            -H "Content-Type: application/octet-stream" \
            $UPLOAD_URL?name=$DEB \
            -d @$DEB
    fi

    RPM=$(ls -1 *.rpm 2>/dev/null)
    if [[ ! -z "$RPM" ]]; then
        echo "Uploading $RPM"
        curl -u $GITHUB_AUTH \
            -H "Content-Type: application/octet-stream" \
            $UPLOAD_URL?name=$RPM \
            -d @$RPM
    fi

    TGZ=$(ls -1 *.tgz 2>/dev/null)
    if [[ ! -z "$TGZ" ]]; then
        echo "Uploading $TGZ"
        curl -u $GITHUB_AUTH \
            -H "Content-Type: application/octet-stream" \
            $UPLOAD_URL?name=$TGZ \
            -d @$TGZ
    fi
fi