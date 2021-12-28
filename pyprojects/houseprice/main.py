import sys
sys.path.append("../../")
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np

from sklearn.preprocessing import OneHotEncoder, LabelEncoder
from sklearn.preprocessing import StandardScaler
from sklearn.impute import SimpleImputer
from sklearn.model_selection import GridSearchCV

from pyprojects import myhelp



def ReadAll():
	df_train = pd.read_csv("data/train.csv", index_col='Id')
	df_test  = pd.read_csv("data/test.csv", index_col='Id')

	return df_train, df_test



def Clean(df, dict_im=None, dict_le=None, dict_ss=None):
    # drop useless cols #
    cols_to_drop = ['Name', 'Cabin', 'Ticket']
    df = df.drop(columns=cols_to_drop)

    # drop missing data #
    if not (dict_im or dict_le or dict_ss):
        print( "Dropping NaN rows..." )
        cols_to_dropna = ['Embarked']
        df = df.dropna(subset=cols_to_dropna)

    # fill missing data #
    cols_to_fillmedian = ['Age']
    cols_to_fillmost = ['Fare']
    if dict_im:
        for c, im in dict_im.items():
            df[c] = im.transform(df[c])
    else:
        dict_im = {}
        for c in cols_to_fillmedian:
            im = SimpleImputer(strategy='median')
            df[c] = im.fit_transform(df[c].to_numpy().reshape([-1,1]))
            dict_im[c] = im
        for c in cols_to_fillmost:
            im = SimpleImputer(strategy='most_frequent')
            df[c] = im.fit_transform(df[c].to_numpy().reshape([-1,1]))
            dict_im[c] = im
    
    # label encode #
    cols_to_le = ['Sex', 'Embarked', 'Pclass']
    if dict_le:
        for c, le in dict_le.items():
            df[c] = le.transform(df[c])
    else:
        dict_le = {}
        for c in cols_to_le:
            le = LabelEncoder()
            df[c] = le.fit_transform(df[c])
            dict_le[c] = le

    # standard scale #
    cols_to_ss = ['Age', 'Fare']
    if dict_ss:
        for c, ss in dict_ss.items():
            df[c] = ss.transform(df[c].to_numpy().reshape([-1,1]))
    else:
        dict_ss = {}
        for c in cols_to_ss:
            ss = StandardScaler()
            df[c] = ss.fit_transform(df[c].to_numpy().reshape([-1,1]))
            dict_ss[c] = ss

    return df, dict_im, dict_le, dict_ss



def LRLearn(X_train, Y_train, X_test, Y_test):
    from sklearn.linear_model import LogisticRegression
    result = { 'LogisticRegression':{} }

    params = {'penalty': ['none', 'l2', 'l1', 'elasticnet'], \
              'C': np.logspace(-2, 2, 10), \
    }
    lr = LogisticRegression(max_iter=5e5)
    lr_cv = GridSearchCV(lr, params, n_jobs=-1, cv=5)
    lr_cv.fit(X_train, Y_train.reshape([-1]))
    result['LogisticRegression']['score'] = lr_cv.best_score_
    result['LogisticRegression'].update( lr_cv.best_params_)

    return result, {'LogisticRegression': lr_cv}



def KNNLearn(X_train, Y_train, X_test, Y_test):
    from sklearn.neighbors import KNeighborsClassifier 
    result = { 'KNN': {'score':0.} }

    params = {'n_neighbors': np.arange(1, 100), \
              'weights': ['uniform', 'distance'], \
              'leaf_size': [10, 20, 30, 40, 50], \
              'p': [1, 2], \
    }
    knn = KNeighborsClassifier()
    knn_cv = GridSearchCV(knn, params, n_jobs=-1, cv=5)
    knn_cv.fit(X_train, Y_train.reshape([-1]))
    result['KNN']['score'] = knn_cv.best_score_
    result['KNN'].update( knn_cv.best_params_ )

    return result, {'KNN': knn_cv}



def SVMLearn(X_train, Y_train, X_test, Y_test):
    from sklearn.svm import LinearSVC
    from sklearn.svm import NuSVC
    from sklearn.svm import SVC 
    result = { 'LinearSVC': {'score':0.}, \
               'NuSVC': {'score':0.}, \
               'SVC': {'score':0.}, \
    }

    # LinearSVC #
    params = {'penalty': ['l1', 'l2'], \
              'loss': ['hinge', 'squared_hinge'], \
              'C': np.logspace(-2, 2., 10), \
    }
    lsvc = LinearSVC(max_iter=5e5, random_state=7)
    lsvc_cv = GridSearchCV(lsvc, params, n_jobs=-1, cv=5)
    lsvc_cv.fit(X_train, Y_train.reshape([-1]))
    result['LinearSVC']['score'] = lsvc_cv.best_score_
    result['LinearSVC'].update( lsvc_cv.best_params_ )

    # NuSVC # 
    params = {'nu': np.linspace(0.1, 1., 10), \
              'kernel': ['linear', 'poly', 'rbf', 'sigmoid'], \
    }
    nsvc = NuSVC(max_iter=5e5, random_state=7)
    nsvc_cv = GridSearchCV(nsvc, params, n_jobs=-1, cv=5)
    nsvc_cv.fit(X_train, Y_train.reshape([-1]))
    result['NuSVC']['score'] = nsvc_cv.best_score_
    result['NuSVC'].update( nsvc_cv.best_params_ )

    # SVC #
    params = {'C': np.logspace(-2, 2., 10), \
              'kernel': ['linear', 'poly', 'rbf', 'sigmoid'], \
    }
    svc = SVC(max_iter=5e5, random_state=7)
    svc_cv = GridSearchCV(svc, params, n_jobs=-1, cv=5)
    svc_cv.fit(X_train, Y_train.reshape([-1]))
    result['SVC']['score'] = svc_cv.best_score_
    result['SVC'].update( svc_cv.best_params_ )

    return result, {'LinearSVC': lsvc_cv, 'NuSVC': nsvc_cv, 'SVC': svc_cv}



def RFLearn(X_train, Y_train, X_test, Y_test):
    from sklearn.ensemble import RandomForestClassifier
    result = { 'RandomForest': {'score':0.} }
    
    params = {'n_estimators': np.arange(2, 100), \
    }
    rf = RandomForestClassifier(random_state=7)
    rf_cv = GridSearchCV(rf, params, n_jobs=-1, cv=5)
    rf_cv.fit(X_train, Y_train.reshape([-1]))
    result['RandomForest']['score'] = rf_cv.best_score_
    result['RandomForest'].update( rf_cv.best_params_ )

    return result, {'RandomForest': rf_cv}




def main():
    df_train, df_test = ReadAll()

    cols_category = ['MSSubClass', 'MSZoning', 'Street', 'Alley', 'LotShape', 'LandContour', \
                     'Utilities', 'LotConfig', 'LandSlope', 'Neighborhood', 'Condition1', 'Condition2', \
                     'BldgType', 'HouseStyle', 'OverallQual', 'OverallCond', 'RoofStyle', \
                     'RoofMatl', 'Exterior1st', 'Exterior2nd', 'MasVnrType', 'ExterQual', \
                     'ExterCond', 'Foundation', 'BsmtQual', 'BsmtCond', 'BsmtExposure', \
                     'BsmtFinType1', 'BsmtFinType2', 'Heating', 'HeatingQC', 'CentralAir', \
                     'Electrical', 'KitchenQual', 'Functional', 'FireplaceQu', 'GarageType', \
                     'GarageFinish', 'GarageQual', 'GarageCond', 'PavedDrive', 'PoolQC', 'Fence', \
                     'MiscFeature', 'SaleType', 'SaleCondition', \
                     ]
    df_train = myhelp.SetDatatype(df_train, cols_category)
#    df_test  = myhelp.SetDatatype(df_test,  cols_category)

    cols_target = ['SalePrice']
    myhelp.Preview(df_train, cols_target=cols_target, output_figs=True)

    # clean #
    df_train, im, le, ss = Clean(df_train)
    X_train = df_train.drop(cols_target, axis=1)
    Y_train = df_train[cols_target]

    # train #
    info, models = myhelp.Learn(X_train.to_numpy(), Y_train.to_numpy(), function_list=[LRLearn, KNNLearn, SVMLearn,RFLearn])
    for name, score in info.items():
        myhelp.Subsection(text=name)
        print(score)

    # work on submission result #
    cols_train = X_train.columns
    print( "Working cols = ", cols_train )

    df_test, _, _, _ = Clean(df_test, dict_le=le, dict_ss=ss)
    myhelp.Preview(df_test, output_prefix='test after cleaning')
    X_test = df_test[cols_train]

    model = models['NuSVC']
    Y_pred = model.predict(X_test)

    submission = pd.DataFrame({
        "PassengerId": df_test.index,
        "Survived": Y_pred
    })
    submission.to_csv('submission.csv', index=False)

    return
















if __name__=="__main__":
    main()
